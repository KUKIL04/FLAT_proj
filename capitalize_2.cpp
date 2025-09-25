#include <fst/fstlib.h>
#include <iostream>
#include <string>
#include <cctype>

using namespace fst;

int main(int argc, char **argv) {
    if (argc != 2) { std::cerr << "Usage: " << argv[0] << " <string>\n"; return 1; }
    std::string input = argv[1];

    StdVectorFst fst;
    int start = fst.AddState(); fst.SetStart(start);
    int current = start;

    for (char c : input) {
        int next = fst.AddState();
        fst.AddArc(current, StdArc(c, std::toupper(c), TropicalWeight::One(), next));
        current = next;
    }
    fst.SetFinal(current, TropicalWeight::One());

    // Traverse FST to get converted string
    std::string output;
    int state = fst.Start();
    while (fst.NumArcs(state) > 0) {
        const StdArc &arc = ArcIterator<StdVectorFst>(fst, state).Value();
        output.push_back((char)arc.olabel);
        state = arc.nextstate;
    }

    std::cout << "Input: " << input << "\nUppercase: " << output << "\n\n";

    // Diagram
    auto draw = [](const StdVectorFst &f, const std::string &title) {
        std::cout << title << "\n->(" << f.Start() << ")";
        int s = f.Start(); while (f.NumArcs(s) > 0) {
            const StdArc &a = ArcIterator<StdVectorFst>(f, s).Value();
            std::cout << " --'" << (char)a.ilabel << "/" << (char)a.olabel << "'--> (" << a.nextstate << ")";
            s = a.nextstate;
        }
        if (f.Final(s) != TropicalWeight::Zero()) std::cout << "*";
        std::cout << "\n\n";
    };

    draw(fst, "Case Conversion FST:");
}

