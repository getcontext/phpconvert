# phpconvert
converts zend 1 type names to namespace style names, 
transforms classes , interfaces from Old_Style_Namespace to \New\Style\Namespace , 
adds usages like 'use \Zend\Db\Sql\Abstract\Class as DbSqlAbstractClass;' and 'namespace \Zend\Db\Sql\Abstract;' (zum beispiel)
tested with PhpStorm 8,Win64

before running on LINUX, export path to BOOST as follows :
export LD_LIBRARY_PATH=/path/to/boost/stage/lib:$LD_LIBRARY_PATH

copy builtintypes.txt and keywords.txt, where executable resides

example command : -d D:/www/zend_framework -o D:/phpconvert/zend_framework -r

run options : 

-d - source project directory
-o - output directory
-r - run recursively


