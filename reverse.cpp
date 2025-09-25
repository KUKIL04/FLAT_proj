#include <fst/fstlib.h>
#include <iostream>
#include <string>

using namespace fst;

int main(int argc, char **argv) {
    if (argc != 2) { std::cerr << "Usage: " << argv[0] << " <string>\n"; return 1; }
    std::string input = argv[1];

    StdVectorFst fst;
    int start = fst.AddState(); fst.SetStart(start);
    int current = start;
    for (char c : input) { int next = fst.AddState(); fst.AddArc(current, StdArc(c, c, TropicalWeight::One(), next)); current = next; }
    fst.SetFinal(current, TropicalWeight::One());

    StdVectorFst rev; Reverse(fst, &rev); RmEpsilon(&rev);

    // Collect reversed string
    std::string output; int state = rev.Start();
    while (rev.NumArcs(state) > 0) {
        const StdArc &arc = ArcIterator<StdVectorFst>(rev, state).Value();
        output.push_back((char)arc.ilabel); state = arc.nextstate;
    }

    std::cout << "Input: " << input << "\nReversed: " << output << "\n\n";

    // Diagram printer
    auto draw = [](const StdVectorFst &f, const std::string &title) {
        std::cout << title << "\n->(" << f.Start() << ")";
        int s = f.Start(); while (f.NumArcs(s) > 0) { const StdArc &a = ArcIterator<StdVectorFst>(f, s).Value(); std::cout << " --'" << (char)a.ilabel << "'--> (" << a.nextstate << ")"; s = a.nextstate; }
        if (f.Final(s) != TropicalWeight::Zero()) std::cout << "*";
        std::cout << "\n\n";
    };

    draw(fst, "Original FST:"); draw(rev, "Reversed FST:");
}

