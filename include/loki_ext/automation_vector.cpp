//#include "StdAfx.h"
#include "automation_vector.h"
#include <strstream>

automation_vector_base::automation_vector_base(unsigned Elements, VARENUM VarType) 
	throw(std::invalid_argument, std::runtime_error)
{
	_ASSERT(V_VT(&m_Value) == VT_EMPTY);
	if (Elements == 0)
		// Empty vectors are VT_EMPTY
		return;
	SAFEARRAYBOUND Bounds = { Elements, 0 };
	if ((m_Value.parray = ::SafeArrayCreate(VarType, 1, &Bounds)) == 0)
		com_enforce(E_OUTOFMEMORY);
	m_Value.vt = VT_ARRAY | VarType;
	_ASSERT(array().cDims == 1);
	_ASSERT(bounds().cElements == Bounds.cElements);
	_ASSERT(bounds().lLbound == Bounds.lLbound);
	_ASSERT(array().cLocks == 0);
	_ASSERT(valid());
}

automation_vector_base::~automation_vector_base() throw()
{
	_ASSERT(valid());
	_ASSERT(empty() || array().cLocks == 0);
}

#ifdef _DEBUG
bool automation_vector_base::valid() const throw()
{
	if (!this || !_CrtIsValidPointer(this, sizeof(*this), false))
	{
		ATLTRACE(_T("The SafeVector's 'this' value (0x%x) "
			"points to invalid memory.\n"), this);
		return false;
	}
	if (m_Value.vt == VT_EMPTY)
		return true;
	std::basic_string<TCHAR> ErrorMessage;
	if (V_ISBYREF(&m_Value))
		ErrorMessage += _T("It is by reference.\n");
	if (!(m_Value.vt & VT_ARRAY))
		ErrorMessage += _T("It is not a safearray.\n");
	else
	{
		if (array().cDims != 1)
			ErrorMessage += _T("It isn't 1-dimensional.\n");
		if (array().cLocks > 1)
			ErrorMessage += _T("There are multiple locks on it.\n");
	}
	if (ErrorMessage.empty())
		return true;

	ATLTRACE(_T("The SafeVector is invalid due to "
		"the following problem(s):\n%s"), ErrorMessage.c_str());
	return false;
}
#endif

void automation_vector_base::com_enforce(HRESULT hr)
{
	if (SUCCEEDED(hr))
		return;
	std::ostrstream s;
	LPTSTR Message;
	if (::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			0,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPTSTR>(&Message),
			0,
			0))
	{
		s << Message << std::ends;
		LocalFree(Message);
	}
	else
		s << "COM operation failed. HRESULT is " << hr << std::ends;
	_ASSERT(false);
	throw std::runtime_error(s.str());
}

void automation_vector_base::get_element(const VARIANT &Array, long Index, 
	VARIANT &v) throw(std::runtime_error)
{
	_ASSERT(V_ISARRAY(&Array));
	SAFEARRAY &a = array(Array);
	_ASSERT(a.cDims == 1);
	VARENUM ElemType = VARENUM(Array.vt & VT_TYPEMASK);
	com_enforce(::VariantClear(&v));
	// Treat arrays of VARIANTs differently
	if (ElemType == VT_VARIANT)
	{
		_ASSERT(a.cbElements == sizeof(VARIANT));	// Array is corrupt!!!
		// Fetch the element directly int v
		com_enforce(::SafeArrayGetElement(&a, &Index, &v));
	}
	else
	{
		// check if the element of the array can fit into a VARIANT's union
		_ASSERT(a.cbElements <= sizeof(VARIANT) - offsetof(VARIANT, lVal));
		// Fetch the element
		com_enforce(::SafeArrayGetElement(&a, &Index, 
			static_cast<void *>(&v.lVal)));
		v.vt = ElemType;
	}
}

void automation_vector_base::put_element(VARIANT &Array, long Index, const VARIANT &v) 
	throw(std::runtime_error)
{
	_ASSERT(V_ISARRAY(&Array));
	SAFEARRAY &a = array(Array);
	VARENUM ElemType = VARENUM(Array.vt & VT_TYPEMASK);
	_ASSERT(ElemType == v.vt || ElemType == VT_VARIANT);
	// Treat arrays of VARIANTs differently
	const void *ToPut;
	switch (ElemType)
	{
	case VT_VARIANT:
		_ASSERT(a.cbElements == sizeof(VARIANT));	// Array is corrupt!!!
		// Put the variant directly into array
		ToPut = &v;
		break;
	case VT_BSTR:
	case VT_DISPATCH:
	case VT_UNKNOWN:
		// check if the element of the array can fit into a VARIANT's union
		_ASSERT(a.cbElements == sizeof(void *));	// Array is corrupt!!!
		ToPut = v.byref;
		break;
	default:
		// check if the element of the array can fit into a VARIANT's union
		_ASSERT(a.cbElements <= sizeof(VARIANT) - offsetof(VARIANT, lVal));
		ToPut = &v.lVal;
		break;
	}
	// Put the element
	com_enforce(::SafeArrayPutElement(&a, &Index, 
		const_cast<void *>(ToPut)));
}

void automation_vector_base::attach(VARIANT &v) throw()
{
	clear();
	std::swap(static_cast<VARIANT &>(m_Value), v);
}

void automation_vector_base::detach(VARIANT &v) throw()
{
	_ASSERT(v.vt == VT_EMPTY);
	_ASSERT(empty() || array().cLocks == 0);
	std::swap(static_cast<VARIANT &>(m_Value), v);
}


void automation_vector_base::resize(size_type NewSize, VARENUM Type)
{
	_ASSERT(valid());
	if (NewSize == 0)
	{
		clear();
		return;
	}
	if (empty())
	{
		_ASSERT(plain_vartype() == VT_EMPTY);
		new(this) self(NewSize, Type);
		return;
	}
	unsigned OldSize = size();
	if (size() == NewSize)
		return;
	SAFEARRAYBOUND bounds = array().rgsabound[0];
	bounds.cElements = NewSize;
	_ASSERT(array().cLocks == 0);
	com_enforce(::SafeArrayRedim(&array(), &bounds));
}


