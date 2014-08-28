#define USE_DINFINITY

#ifdef USE_DINFINITY
#include "DInfinityMap.h"
#define DIRMAP_TYPE DInfinityMap
#else
#include "D8Map.h"
#define DIRMAP_TYPE D8Map
#endif

using namespace openworld;

unsigned findBasin(DIRMAP_TYPE& map, MatrixGeographicMap<bool>& mask, unsigned rrinit, unsigned ccinit);
void checkCell(unsigned rr_from, unsigned cc_from, unsigned rr_to, unsigned cc_to, DIRMAP_TYPE& map, queue< pair<unsigned, unsigned> >& pending, GeographicMap<bool>& mask);

// call as basinmask ang.tiff mask.tsv OUTLAT OUTLON LAT0 LAT1 DLAT LON0 LON1 DLON
int main(int argc, const char* argv[]) {
  MPI_Init(NULL,NULL); {
    int rank,size;
    MPI_Comm_rank(MCW,&rank);
    MPI_Comm_size(MCW,&size);

    double lat0 = atof(argv[5]);
    double lat1 = atof(argv[6]);
    double dlat = atof(argv[7]);
    double lon0 = atof(argv[8]);
    double lon1 = atof(argv[9]);
    double dlon = atof(argv[10]);

    DIRMAP_TYPE map(*MatrixGeographicMap<float>::loadTIFF(DividedRange::withEnds(lat0, lat1, dlat, Inds::lat),
							  DividedRange::withEnds(lon0, lon1, dlon, Inds::lon),
							  argv[1]), false);
    /*for (unsigned rr = 1; rr < 10; rr++) {
      for (unsigned cc = 1; cc < 10; cc++)
	cout << map.getCellConst(rr, cc) << ", ";
      cout << endl;
      }*/

    MatrixGeographicMap<bool> mask(map.getLatitudes(), map.getLongitudes());

    unsigned rrinit = map.getLatitudes().inRange(atof(argv[3])), ccinit = map.getLongitudes().inRange(atof(argv[4]));

    if (rrinit == (unsigned) -1) {
      unsigned maxcount = 0;
      Measure bestLatitude(0, Inds::lat);
      unsigned bestrow = 0;
      for (rrinit = 0; rrinit < map.getLatitudes().count(); rrinit++) {
        unsigned count = findBasin(map, mask, rrinit, ccinit);
	if (count > maxcount) {
	  maxcount = count;
	  bestLatitude = map.getLatitudes().getCellCenter(rrinit);
	  bestrow = rrinit;
	}
	mask.loadConstantInto(0);
      }

      cout << "BEST: " << bestLatitude << " (" << bestrow << ") has total cells " << maxcount << endl;
    } else if (ccinit == (unsigned) -1) {
      unsigned maxcount = 0;
      Measure bestLongitude(0, Inds::lon);
      unsigned bestcol = 0;
      for (ccinit = 0; ccinit < map.getLongitudes().count(); ccinit++) {
        unsigned count = findBasin(map, mask, rrinit, ccinit);
	if (count > maxcount) {
	  maxcount = count;
	  bestLongitude = map.getLongitudes().getCellCenter(ccinit);
	  bestcol = ccinit;
	}
	mask.loadConstantInto(0);
      }

      cout << "BEST: " << bestLongitude << " (" << bestcol << ") has total cells " << maxcount << endl;
    } else {
      findBasin(map, mask, rrinit, ccinit);
      mask.loadConstantInto(0);
      findBasin(map, mask, rrinit, ccinit);

      mask.getValues().saveDelimited(argv[2], FileFormatter<bool>::formatSimple, '\t');
    }

  } MPI_Finalize();
}

unsigned findBasin(DIRMAP_TYPE& map, MatrixGeographicMap<bool>& mask, unsigned rrinit, unsigned ccinit) {
  cout << "Flow to " << rrinit << ", " << ccinit;

  mask.getCell(rrinit, ccinit) = true;

  queue< pair<unsigned, unsigned> > pending;
  pending.push(pair<unsigned, unsigned>(rrinit, ccinit));

  unsigned count = 0;
  while (!pending.empty()) {
    count++;
    pair<unsigned, unsigned> rc = pending.front();
    //cout << "  " << rc.first << ", " << rc.second << endl;
      
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
  return count;
}

void checkCell(unsigned rr_from, unsigned cc_from, unsigned rr_to, unsigned cc_to, DIRMAP_TYPE& map, queue< pair<unsigned, unsigned> >& pending, GeographicMap<bool>& mask) {
  if (rr_from < 0 || rr_from >= map.getLatitudes().count() || cc_from < 0 || cc_from >= map.getLongitudes().count())
    return;
  if (mask.getCellConst(rr_from, cc_from))
    return;

  /*unsigned rr0, cc0, rr1, cc1;
  double portion0;

  map.getDirections(rr_from, cc_from, rr0, cc0, rr1, cc1, portion0);
  cout << "CHECK: " << map.getCellConst(rr_from, cc_from) << ": " << rr0 << ", " << cc0 << ", " << rr1 << ", " << cc1 << ", " << portion0 << endl;
  map.getDirections(rr_from, cc_from, rr0, cc0, rr1, cc1, portion0);
  cout << "CHECK: " << map.getCellConst(rr_from, cc_from) << ": " << rr0 << ", " << cc0 << ", " << rr1 << ", " << cc1 << ", " << portion0 << endl;
  cout << "ONE: " << map.flowsInto(rr_from, cc_from, rr_to, cc_to) << endl;
  cout << "TWO: " << map.flowsInto(rr_from, cc_from, rr_to, cc_to) << endl;*/

  //cout << "Check: " << rr_from << ", " << cc_from << " -> " << rr_to << ", " << cc_to << " :: " << map.flowsInto(rr_from, cc_from, rr_to, cc_to) << endl;
  if (map.flowsInto(rr_from, cc_from, rr_to, cc_to)) {
    mask.getCell(rr_from, cc_from) = true;
    pending.push(pair<unsigned, unsigned>(rr_from, cc_from));
    //cout << "Add " << rr_from << ", " << cc_from << endl;
  }
}
