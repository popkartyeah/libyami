/*
 *  decodehelp.h - decode test help
 *
 *  Copyright (C) 2011-2014 Intel Corporation
 *    Author: Halley Zhao<halley.zhao@intel.com>
 *             Xin Tang<xin.t.tang@intel.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301 USA
 */
#ifndef __DECODE_HELP__
#define __DECODE_HELP__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cpluscplus
extern "C" {
#endif

#include "interface/VideoDecoderDefs.h"
#ifdef __ENABLE_CAPI__
#include "capi/VideoDecoderCapi.h"
#else
#include "interface/VideoDecoderHost.h"
#endif
#include "common/log.h"
#include <unistd.h>


#ifndef VA_FOURCC_I420
#define VA_FOURCC_I420 VA_FOURCC('I','4','2','0')
#endif
char *dumpOutputDir = NULL;
uint32_t dumpFourcc = VA_FOURCC_I420;
int renderMode = 1;
bool waitBeforeQuit = false;
char *inputFileName = NULL;


static void print_help(const char* app)
{
    printf("%s <options>\n", app);
    printf("   -i media file to decode\n");
    printf("   -w wait before quit\n");
    printf("   -f dumped fourcc\n");
    printf("   -o dumped output dir\n");
    printf("   -m <render mode>\n");
    printf("      0: dump video frame to file\n");
    printf("      1: render to X window\n");
    printf("      2: texture: render to Pixmap + texture from Pixmap\n");
    printf("      3: texture: export video frame as drm name (RGBX) + texture from drm name\n");
    printf("      4: texture: export video frame as dma_buf(RGBX) + texutre from dma_buf\n");
    printf("      5: texture: export video frame as dma_buf(NV12) + texture from dma_buf\n");
}

static bool process_cmdline(int argc, char *argv[])
{
    char opt;

    while ((opt = getopt(argc, argv, "h:m:i:f:o:w?")) != -1)
    {
        switch (opt) {
        case 'h':
        case '?':
            print_help (argv[0]);
            return false;
        case 'i':
            inputFileName = optarg;
            break;
        case 'w':
            waitBeforeQuit = true;
            break;
        case 'm':
            renderMode = atoi(optarg);
            break;
        case 'f':
            if (strlen(optarg) == 4) {
                dumpFourcc = VA_FOURCC(optarg[0], optarg[1], optarg[2], optarg[3]);
            } else {
                fprintf(stderr, "invalid fourcc: %s\n", optarg);
            }
            break;
        case 'o':
            if (optarg)
                dumpOutputDir = strdup(optarg);
            break;
        default:
            print_help(argv[0]);
            break;
        }
    }
    if (!inputFileName) {
        fprintf(stderr, "no input media file specified\n");
        return -1;
    }
    fprintf(stderr, "input file: %s, renderMode: %d\n", inputFileName, renderMode);

    if (!dumpOutputDir)
        dumpOutputDir = strdup ("./");

#ifndef __ENABLE_TESTS_GLES__
    if (renderMode > 1) {
        fprintf(stderr, "renderMode=%d is not supported, please rebuild with --enable-tests-gles option\n", renderMode);
        return -1;
    }
#endif
    return true;
}

#endif