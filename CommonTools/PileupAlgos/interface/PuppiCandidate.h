#ifndef CommonTools_PileupAlgos_PuppiCandidate
#define CommonTools_PileupAlgos_PuppiCandidate

#include "fastjet/PseudoJet.hh"
#include <vector>

const double pseudojet_invalid_eta = -1e200;

// Extension of fastjet::PseudoJet that caches eta and some other info
// Puppi uses register to decide between NHs, PV CHs, and PU CHs.
class PuppiCandidate : public fastjet::PseudoJet {
  public:
    using fastjet::PseudoJet::PseudoJet;
    double pseudorapidity() const { _ensure_valid_eta(); return _eta; }
    double eta() const { return pseudorapidity(); }
    void _ensure_valid_eta() const { if(_eta==pseudojet_invalid_eta) _eta = fastjet::PseudoJet::pseudorapidity(); }
    void set_info(int puppi_register, bool charged) { puppi_register_ = puppi_register; charged_ = charged; }
    inline int puppi_register() const { return puppi_register_; }
    inline bool charged() const { return charged_; }
    void dist_resize(unsigned n) { dists_.resize(n,0.); }
    void set_dist(unsigned i, double d) { dists_[i] = d; }
    inline double dist(unsigned i) const { return dists_[i]; }
  private:
    mutable double _eta = pseudojet_invalid_eta;
    int puppi_register_;
    bool charged_;
    std::vector<double> dists_;
};

#endif
