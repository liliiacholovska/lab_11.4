#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab11.4/lab11.4.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
        TEST_METHOD(TestSortByPhoneNumber)
        {
            Note notes[] = {
        { "John", "Doe", "555-333-200", {1, 2, 2000} },
        { "Jane", "Smith", "123-456-789", {3, 4, 1999} },
        { "Robert", "Johnson", "999-888-777", {5, 6, 2001} }
            };
            writeNotes("test_notes.dat", notes, 3);
            std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
            std::ostringstream strCout;
            std::cout.rdbuf(strCout.rdbuf());
            sortNotesByPhoneNumber("test_notes.dat");
            readAndPrintNotes("test_notes.dat"); 
            std::cout.rdbuf(oldCoutStreamBuf);
            std::string output = strCout.str();
 
            Assert::IsTrue(output.find("123-456-789") != std::string::npos);
            Assert::IsTrue(output.find("555-333-200") != std::string::npos);
            Assert::IsTrue(output.find("999-888-777") != std::string::npos);
        }

        TEST_METHOD(TestSearchByLastName)
        {
            Note notes[] = {
                { "John", "Doe", "555-333-200", {1, 2, 2000} },
                { "Jane", "Smith", "123-456-789", {3, 4, 1999} },
                { "Robert", "Johnson", "999-888-777", {5, 6, 2001} }
            };
            writeNotes("test_notes.dat", notes, 3);
            Note foundNote;
            bool result = searchByLastName("test_notes.dat", "Smith", foundNote);

            Assert::IsTrue(result);
            Assert::AreEqual(string("Jane"), string(foundNote.firstName));
            Assert::AreEqual(string("Smith"), string(foundNote.lastName));
            Assert::AreEqual(string("123-456-789"), string(foundNote.phoneNumber));
        }
	};
}
