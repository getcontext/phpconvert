# zf2ns
converts zend 1 type names to namespace style names, 
transforms classes , interfaces from Old_Style_Namespace to \New\Style\Namespace , 
adds usages like 'use \Zend\Db\Sql\Abstract\Class as DbSqlAbstractClass;' and 'namespace \Zend\Db\Sql\Abstract;' (zum beispiel)
tested with PhpStorm 8,Win64 

example command : -d D:/www/digitalus_cms -o D:/zf2ns/digitalus_cms -r

run options : 

-d - source project directory
-o - output directory
-r - run recursively


