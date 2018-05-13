//
// Created by mrjaqbq on 05.03.16.
//

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <trompeloeil.hpp>
namespace trompeloeil
{
    template <>
    void reporter<specialized>::send(
            severity s,
            const char* file,
            unsigned long line,
            const char* msg)
    {
        std::ostringstream os;
        if (line) os << file << ':' << line << '\n';
        os << msg;
        auto failure = os.str();
        Catch::AssertionHandler catchAssertionHandler("REQUIRED_CALL", Catch::SourceLineInfo(file, line), failure, Catch::ResultDisposition::Normal );
        INTERNAL_CATCH_TRY {
            CATCH_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS
            catchAssertionHandler.handleMessage(s == severity::fatal ? Catch::ResultWas::OfType::FatalErrorCondition : Catch::ResultWas::OfType::ExpressionFailed, failure);
            CATCH_INTERNAL_UNSUPPRESS_PARENTHESES_WARNINGS
        } INTERNAL_CATCH_CATCH(catchAssertionHandler) {
            INTERNAL_CATCH_REACT(catchAssertionHandler)
        }
    }
}

//namespace trompeloeil
//{
//    template <>
//    void reporter<specialized>::send(
//        severity s,
//        const char* file,
//        unsigned long line,
//        const char* msg)
//    {
//        std::ostringstream os;
//        if (line) os << file << ':' << line << '\n';
//        os << msg;
//        auto failure = os.str();
//        if (s == severity::fatal)
//        {
//            FAIL(failure);
//        }
//        else
//        {
//            CAPTURE(failure);
//            CHECK(failure.empty());
//        }
//    }
//}

//int main(int argc, char **argv)
//{
//	int result = Catch::Session().run(argc, argv);
//	printf("Catch2 finished with: %d", result);
//	return 0;
//}