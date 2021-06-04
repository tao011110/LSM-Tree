#include <iostream>
#include <cstdint>
#include <string>
#include <cassert>

#include "test.h"

class PersistenceTest : public Test {
private:
    const uint64_t TEST_MAX = 1024 * 32;
    void prepare(uint64_t max)
    {
        store.isdebug = false;
        uint64_t i;

        // Clean up
        store.reset();

        // Test multiple key-value pairs
        for (i = 0; i < max; ++i) {
            store.put(i, std::string(i+1, 's'));
            EXPECT(std::string(i+1, 's'), store.get(i));
        }
        phase();

        // Test after all insertions
        for (i = 0; i < max; ++i)
            EXPECT(std::string(i+1, 's'), store.get(i));
        phase();

        // Test deletions
        for (i = 0; i < max; i+=2) {
            //std::cout << i << std::endl;
            EXPECT(true, store.del(i));
        }
        std::cout << "finish part 1" << std::endl;
        // Prepare data for Test Mode
        for (i = 0; i < max; ++i) {
            switch (i & 3) {
                case 0:
                    EXPECT(not_found, store.get(i));
                    store.put(i, std::string(i+1, 't'));
                    break;
                case 1:
                    EXPECT(std::string(i+1, 's'), store.get(i));
                    store.put(i, std::string(i+1, 't'));
                    break;
                case 2:
                    EXPECT(not_found, store.get(i));
                    break;
                case 3:
                    EXPECT(std::string(i+1, 's'), store.get(i));
                    break;
                default:
                    assert(0);
            }
        }

        phase();

        report();
        //store.isdebug = true;

        /**
         * Write 10MB data to drain previous data out of memory.
         */
        for (i = 0; i <= 10240; ++i)
            store.put(max + i, std::string(1024, 'x'));

        std::cout << "Data is ready, please press ctrl-c/ctrl-d to"
                     " terminate this program!" << std::endl;
        std::cout.flush();
        //test(max);
        for (int i = 0; i < 5; i++) {
            volatile int dummy;
            for (i = 0; i <= 1024; ++i) {
                // The loop slows down the program
                for (i = 0; i <= 1000; ++i)
                    dummy = i;

                store.del(max + i);

                for (i = 0; i <= 1000; ++i)
                    dummy = i;

                store.put(max + i, std::string(1024, '.'));

                for (i = 0; i <= 1000; ++i)
                    dummy = i;

                store.put(max + i, std::string(512, 'x'));
            }
        }
    }

    void test(uint64_t max)
    {
        store.isdebug = true;
        uint64_t i;
        // Test data
        for (i = 0; i < max; ++i) {
            switch (i & 3) {
                case 0: {
                    if(i +1 == 10777){
                        std::cout << "wo"<<std::endl;
                    }
                    bool flag = EXPECT(std::string(i + 1, 't'), store.get(i));
                    if(flag == false){
                        std::cout <<store.get(i).length() << "  w  " << i + 1 << std::endl;
                    }
                    break;
                }
                case 1: {
                    bool flag = EXPECT(std::string(i + 1, 't'), store.get(i));
                    if(flag == false){
                        std::cout <<store.get(i).length() << "  ww  " << i + 1 << std::endl;
                    }
                    break;
                }
                case 2: {
                    if(i +1 == 3){
                        std::cout << "wo"<<std::endl;
                    }
                    bool flag = EXPECT(not_found, store.get(i));
                    if(flag == false){
                        std::cout <<store.get(i).length() << "  www  " << i + 1 << std::endl;
                    }
                    break;
                }
                case 3: {
                    if(i +1 == 14400 || i+ 1 == 14404){
                        std::cout << "wo"<<std::endl;
                    }
                    bool flag = EXPECT(std::string(i + 1, 's'), store.get(i));
                    if(flag == false){
                        std::cout << store.get(i).length() <<"  wwww  " << i + 1 << std::endl;
                    }
                    break;
                }
                default:
                    assert(0);
            }
        }

        phase();

        report();
    }

public:
    PersistenceTest(const std::string &dir, bool v=true) : Test(dir, v)
    {
    }

    void start_test(void *args = NULL) override
    {
        bool testmode = (args && *static_cast<bool *>(args));

        std::cout << "KVStore Persistence Test" << std::endl;

        if (testmode) {
            std::cout << "<<Test Mode>>" << std::endl;
            test(TEST_MAX);
        } else {
            std::cout << "<<Preparation Mode>>" << std::endl;
            prepare(TEST_MAX);
        }
    }
};

void usage(const char *prog, const char *verb, const char *mode)
{
    std::cout << "Usage: " << prog  << " [-t] [-v]" << std::endl;
    std::cout << "  -t: test mode for persistence test,"
                 " if -t is not given, the program only prepares data for test."
                 " [currently " << mode << "]" << std::endl;
    std::cout << "  -v: print extra info for failed tests [currently ";
    std::cout << verb << "]" << std::endl;
    std::cout << std::endl;
    std::cout << " NOTE: A normal usage is as follows:" << std::endl;
    std::cout << "    1. invoke `" << prog << "`;" << std::endl;
    std::cout << "    2. terminate (kill) the program when data is ready;";
    std::cout << std::endl;
    std::cout << "    3. invoke `" << prog << "-t ` to test." << std::endl;
    std::cout << std::endl;
    std::cout.flush();
}


int main(int argc, char *argv[])
{
    bool verbose = true;
    bool testmode = false;
    if (argc == 2) {
        verbose = std::string(argv[1]) == "-v";
        testmode = std::string(argv[1]) == "-t";
    } else if (argc == 3) {
        verbose = std::string(argv[1]) == "-v" ||
                  std::string(argv[2]) == "-v";
        testmode = std::string(argv[1]) == "-t" ||
                   std::string(argv[2]) == "-t";
    } else if (argc > 3) {
        std::cerr << "Too many arguments." << std::endl;
        usage(argv[0], "OFF", "Preparation Mode");
        exit(-1);
    }
    usage(argv[0], verbose ? "ON" : "OFF",
          testmode ? "Test Mode" : "Preparation Mode");

    PersistenceTest test("./data", verbose);

    test.start_test(static_cast<void *>(&testmode));

    return 0;
}
