# zf2ns
converts zend 1 type names to namespace style names, 
transforms classes , interfaces from Old_Style_Namespace to \New\Style\Namespace , 
adds usages like 'use \Zend\Db\Sql\Abstract\Class as DbSqlAbstractClass;' and 'namespace \Zend\Db\Sql\Abstract;' (zum beispiel)

tested with
- boost lib
- PhpStorm 8,Win64, Cygwin, gcc, g++ (no cmake);
- Ubuntu 18.04 , 18.10 , 19.10 , CMake 3.15.3 , GDB 8.3 (current version 1.5b)

example command : -d D:/www/digitalus_cms -o D:/zf2ns/digitalus_cms -r

run options : 

-d - source project directory
-o - output directory
-r - run recursively


