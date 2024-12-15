#ifndef MACRO_HPP
#define MACRO_HPP

#define MACRO_VARIABLE_TO_STRING(Variable) (void(Variable),#Variable)
#define MACRO_FUNCTION_TO_STRING(Function) (void(&Function),#Function)
#define MACRO_METHOD_TO_STRING(ClassName,Method) (void(&ClassName::Method),#Method)
#define MACRO_TYPE_TO_STRING(Type) (void(sizeof(Type)),#Type)

#endif // MACRO_HPP
