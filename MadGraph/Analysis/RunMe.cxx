//#include "HiggsAnalysis.C"
#include "HiggsAnalysis_14TeV.C"

//
// "ROOT Script" entry point (the same name as the "filename's base").
//
// [bash/csh] root RunMe.cxx
// [bash/csh] root RunMe.cxx++
// root [0] .x RunMe.cxx
// root [0] .x RunMe.cxx++
//
void RunMe(void)
{
  HiggsAnalysis_14TeV m;
  //HiggsAnalysis m;
  m.Loop();
}

#if !defined(__CINT__) && !defined(__ACLIC__)
//
// "Standalone Application" entry point ("main").
//
// `root-config --cxx --cflags` -o RunMe RunMe.cxx `root-config --libs`
// ./RunMe
//
int main(int /*argc*/, char ** /*argv*/)
{
  RunMe(); // just call the "ROOT Script"
  return 0;
}
#endif /* !defined(__CINT__) && !defined(__ACLIC__) */
