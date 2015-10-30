> The library comes with Unit Tests which test and also provide current coding examples to accomplish anything which it is capable of. Tests are preformed using [Building#Cpp\_Unit](Building#Cpp_Unit.md) and [Building#Google\_Mock](Building#Google_Mock.md), their IDE project files/makefile have an additional dependency of [Building#Premake](Building#Premake.md).

# Organisation #
> Unit test files are organised into directories, with C++ classes in [cpp\_classes](http://code.google.com/p/oolua/source/browse/#svn/trunk/unit_tests/cpp_classes), the binding of these classes in [bind\_classes](http://code.google.com/p/oolua/source/browse/#svn/trunk/unit_tests/bind_classes) and then separate unit test projects for the features being tested under the directory [unit\_tests](http://code.google.com/p/oolua/source/browse/#svn/trunk/unit_tests).

# Running Tests #
> When project files for the tests are generated using Premake, a post build event is added to run the executable. It is also possible to run all tests projects sequentially from the command line using [Scripts](http://code.google.com/p/oolua/wiki/Building#Compile_Scripts).