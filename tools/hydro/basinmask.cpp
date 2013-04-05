#include "DInfinityMap.h"

using namespace openworld;

void findBasin(DInfinityMap& map, MatrixGeographicMap<bool>& mask, unsigned rrinit, unsigned ccinit);
void checkCell(unsigned rr_from, unsigned cc_from, unsigned rr_to, unsigned cc_to, DInfinityMap& map, queue< pair<unsigned, unsigned> >& pending, GeographicMap<bool>& mask);

// call as basinmask ang.tiff mask.tsv 29.00417 34.99583 70.00417 85.99583 29.385316 71.076393
int main(int argc, const char* argv[]) {
  MPI_Init(NULL,NULL); {
	int rank,size;
	MPI_Comm_rank(MCW,&rank);
	MPI_Comm_size(MCW,&size);

    DInfinityMap map(*MatrixGeographicMap<float>::loadTIFF(DividedRange::withEnds(atof(argv[3]), atof(argv[4]), 0.008333334, Inds::lat),
                                                           DividedRange::withEnds(atof(argv[5]), atof(argv[6]), 0.008333334, Inds::lon),
                                                           argv[1]), false);
    MatrixGeographicMap<bool> mask(map.getLatitudes(), map.getLongitudes());

    unsigned rrinit = map.getLatitudes().inRange(atof(argv[7])), ccinit = map.getLongitudes().inRange(atof(argv[8]));

    if (rrinit == (unsigned) -1) {
      for (rrinit = 0; rrinit < map.getLatitudes().count(); rrinit++)
        findBasin(map, mask, rrinit, ccinit);
    } else
      findBasin(map, mask, rrinit, ccinit);

    mask.getValues().saveDelimited(argv[2], FileFormatter<bool>::formatSimple, '\t');
  } MPI_Finalize();
}

void findBasin(DInfinityMap& map, MatrixGeographicMap<bool>& mask, unsigned rrinit, unsigned ccinit) {
  cout << "Flow to " << rrinit << ", " << ccinit;

  mask.getCell(rrinit, ccinit) = true;

  queue< pair<unsigned, unsigned> > pending;
  pending.push(pair<unsigned, unsigned>(rrinit, ccinit));

  unsigned count = 0;
  while (!pending.empty()) {
    count++;
    pair<unsigned, unsigned> rc = pending.front();
      
    checkCell(rc.first - 1, rc.second - 1, rc.first, rc.second, map, pending, mask);
    checkCell(rc.first - 1, rc.second, rc.first, rc.second, map, pending, mask);
    checkCell(rc.first - 1, rc.second + 1, rc.first, rc.second, map, pending, mask);
    checkCell(rc.first, rc.second - 1, rc.first, rc.second, map, pending, mask);
    checkCell(rc.first, rc.second + 1, rc.first, rc.second, map, pending, mask);
    checkCell(rc.first + 1, rc.second - 1, rc.first, rc.second, map, pending, mask);
    checkCell(rc.first + 1, rc.second, rc.first, rc.second, map, pending, mask);
    checkCell(rc.first + 1, rc.second + 1, rc.first, rc.second, map, pending, mask);

    pending.pop();
  }

  cout << ": Total cells: " << count << endl;  
}

void checkCell(unsigned rr_from, unsigned cc_from, unsigned rr_to, unsigned cc_to, DInfinityMap& map, queue< pair<unsigned, unsigned> >& pending, GeographicMap<bool>& mask) {
  if (rr_from < 0 || rr_from >= map.getLatitudes().count() || cc_from < 0 || cc_from >= map.getLongitudes().count())
    return;
  if (mask.getCellConst(rr_from, cc_from))
    return;
  if (map.flowsInto(rr_from, cc_from, rr_to, cc_to)) {
    mask.getCell(rr_from, cc_from) = true;
    pending.push(pair<unsigned, unsigned>(rr_from, cc_from));
  }
}
