#include "BenXMLOutput.h"

int main() {
    BenXMLOutput output("Test.xml");

    output.StartTag("OutputName");

    const auto *pszOutputName = "ITW Calculations";
    output.SerializeData(pszOutputName);

    output.EndTag("OutputName");

    output.Flush();
    output.Close();

    return 0;
}