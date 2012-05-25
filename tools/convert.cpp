#include <stdio.h>
#include "../datastr/Matrix.h"
#include "../datastr/FileFormats.h"
#include "../geotiff/commonLib.h"
#include <mpi.h>

enum format {
  tiff_short,
  tiff_long,
  tiff_float,
  csv_long,
  csv_double,
  tsv_long,
  tsv_double
};

string format_names[] = {"tifs", "tifl", "tiff", "csvi", "csvf", "tsvi", "tsvf", ""};

format identify(const char* filename);
format translate(const char* name);

int main(int argc, const char* argv[])
{
  int format_count = 0;
  const char *formats[2];
  const char *infile = NULL, *outfile = NULL, *tmplfile = NULL;

  printf("Initializing...\n");
  for (int ii = 1; ii < argc; ii++) {
    if (*argv[ii] == '-')
      formats[format_count++] = argv[ii] + 1;
    else if (!infile)
      infile = argv[ii];
    else if (!outfile)
      outfile = argv[ii];
    else if (!tmplfile)
      tmplfile = argv[ii];
    else
      throw runtime_error("Unknown arguments");
  }

  printf("Identifying formats...\n");
  format informat, outformat;
  if (format_count == 0) {
    informat = identify(infile);
    outformat = identify(outfile);
  } else if (format_count == 1 && !outfile) {
    informat = identify(infile);
    outformat = translate(formats[0]);
    // TODO: construct outfile here
  } else if (format_count == 1) {
    informat = identify(infile);
    outformat = translate(formats[0]);
  } else {
    informat = translate(formats[0]);
    outformat = translate(formats[1]);
  }
  if (!tmplfile)
    tmplfile = infile;

  MPI_Init(NULL,NULL); {
	int rank,size;
	MPI_Comm_rank(MCW,&rank);
	MPI_Comm_size(MCW,&size);

    openworld::MatrixAbstract* import;

    printf("Reading input...\n");
    if (informat == tiff_short)
      import = openworld::Matrix<short>::loadTIFF(infile);
    else if (informat == tiff_long)
      import = openworld::Matrix<long>::loadTIFF(infile);
    else if (informat == tiff_float)
      import = openworld::Matrix<float>::loadTIFF(infile);
    else if (informat == csv_long || informat == tsv_long)
      import = openworld::Matrix<long>::loadDelimited(string(infile), FileParser<long>::parseSimple, (informat == csv_long ? ',' : '\t'));
    else if (informat == csv_double || informat == tsv_double)
      import = openworld::Matrix<double>::loadDelimited(infile, FileParser<double>::parseSimple, (informat == csv_double ? ',' : '\t'));

    printf("Writing output...\n");
    if (outformat == tiff_short) {
      openworld::Matrix<short> otport(*import);
      otport.saveTIFF(outfile, tmplfile);
    } else if (outformat == tiff_long) {
      openworld::Matrix<long> otport(*import);
      otport.saveTIFF(outfile, tmplfile);
    } else if (outformat == tiff_float) {
      openworld::Matrix<float> otport(*import);
      otport.saveTIFF(outfile, tmplfile);
    } else if (outformat == csv_long || outformat == tsv_long) {
      openworld::Matrix<long> otport(*import);
      otport.saveDelimited(outfile, FileFormatter<long>::formatSimple, (outformat == csv_long ? ',' : '\t'));
    } else if (outformat == csv_double || outformat == tsv_double) {
      openworld::Matrix<double> otport(*import);
      otport.saveDelimited(outfile, FileFormatter<double>::formatSimple, (outformat == csv_double ? ',' : '\t'));
    }

    //Brackets force MPI-dependent objects to go out of scope before Finalize is called
	} MPI_Finalize();
    printf("Done.\n");
}

format identify(const char* filename) {
  char* ext = strrchr(filename, '.') + 1;
  if (!strcmp(ext, "tif") || !strcmp(ext, "tiff"))
    return tiff_short;
  if (!strcmp(ext, "csv"))
    return csv_double;
  if (!strcmp(ext, "tsv"))
    return tsv_double;
  printf("Unknown file format: %s\n", filename);
  throw runtime_error("Unknown format");
}

format translate(const char* name) {
  for (int ii = 0; !format_names[ii].empty(); ii++) {
    if (!strcmp(format_names[ii].c_str(), name))
      return (format) ii;
  }

  printf("Unknown format name: [%s]\n", name);
  throw runtime_error("Unknown format");
}
