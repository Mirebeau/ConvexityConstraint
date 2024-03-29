
#include "JMM_CPPLibs/Macros/ExportArrow.h"
#include "JMM_CPPLibs/Output/FileIO.h"
typedef IO_<FileIO> IO;
typedef typename IO::Msg Msg;
typedef typename IO::WarnMsg WarnMsg;
#include "Headers/DispatchAndRun.h"

int main(int argc, const char * argv[]) { 
    std::string inputPrefix, outputPrefix;
    
    inputPrefix  = argc > 1 ? argv[1] : "input";
    outputPrefix = argc > 2 ? argv[2] : "output";
    
//	IO io(inputPrefix,outputPrefix);Run(io);return;
	
    try {
        IO io(inputPrefix,outputPrefix);
        Run(io);
    } catch(const std::logic_error & e) {
        IO::WarnMsg() << "Exception caught. " << e.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
