#ifndef Py_INTERNAL_LONG_H
#define Py_INTERNAL_LONG_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

#include "pycore_interp.h"        // PyInterpreterState.small_ints
#include "pycore_pystate.h"       // _PyThreadState_GET()

// Don't call this function but _PyLong_GetZero() and _PyLong_GetOne()
static inline PyObject* __PyLong_GetSmallInt_internal(int value)
{
    PyInterpreterState *interp = _PyInterpreterState_GET();
    assert(-_PY_NSMALLNEGINTS <= value && value < _PY_NSMALLPOSINTS);
    size_t index = _PY_NSMALLNEGINTS + value;
    PyObject *obj = (PyObject*)interp->small_ints[index];
    // _PyLong_GetZero(), _PyLong_GetOne() and get_small_int() must not be
    // called before _PyLong_Init() nor after _PyLong_Fini().
    assert(obj != NULL);
    return obj;
}

// Return a borrowed reference to the zero singleton.
// The function cannot return NULL.
PyAPI_FUNC(PyObject*) _PyLong_GetZero(void);

// Return a borrowed reference to the one singleton.
// The function cannot return NULL.
PyAPI_FUNC(PyObject*) _PyLong_GetOne(void);

#define PY_ZERO() ((PyObject *)_PyInterpreterState_GET()->small_ints[_PY_NSMALLNEGINTS])
#define PY_ONE() ((PyObject *)_PyInterpreterState_GET()->small_ints[_PY_NSMALLNEGINTS+1])

PyObject *_PyLong_Add(PyLongObject *left, PyLongObject *right);
PyObject *_PyLong_Multiply(PyLongObject *left, PyLongObject *right);

/* Used by Python/mystrtoul.c, _PyBytes_FromHex(),
   _PyBytes_DecodeEscape(), etc. */
PyAPI_DATA(unsigned char) _PyLong_DigitValue[256];

/* Format the object based on the format_spec, as defined in PEP 3101
   (Advanced String Formatting). */
PyAPI_FUNC(int) _PyLong_FormatAdvancedWriter(
    _PyUnicodeWriter *writer,
    PyObject *obj,
    PyObject *format_spec,
    Py_ssize_t start,
    Py_ssize_t end);

PyAPI_FUNC(int) _PyLong_FormatWriter(
    _PyUnicodeWriter *writer,
    PyObject *obj,
    int base,
    int alternate);

PyAPI_FUNC(char*) _PyLong_FormatBytesWriter(
    _PyBytesWriter *writer,
    char *str,
    PyObject *obj,
    int base,
    int alternate);

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_LONG_H */
