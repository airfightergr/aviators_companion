//
// Created by ilias on 8/12/23.
//
#define SPNG_STATIC
#include "spng.h"

#
#include <stdio.h>
#include <inttypes.h>



int encode_image(void * image, size_t length, uint32_t width, uint32_t height, enum spng_color_type color_type, int bit_depth)
{
    int fmt;
    int ret = 0;
    spng_ctx *ctx = NULL;
    struct spng_ihdr ihdr= {0};

    /* Creating an encoder context requires a flag */
    ctx = spng_ctx_new(SPNG_CTX_ENCODER);

    /* Encode to internal buffer managed by the library */
    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

    /* Alternatively you can set an output FILE* or stream with spng_set_png_file() or spng_set_png_stream() */

    /* Set image properties, this determines the destination image format */
    ihdr.width = width;
    ihdr.height = height;
    ihdr.color_type = color_type;
    ihdr.bit_depth = bit_depth;
    /* Valid color type, bit depth combinations: https://www.w3.org/TR/2003/REC-PNG-20031110/#table111 */

    spng_set_ihdr(ctx, &ihdr);

    /* When encoding fmt is the source format */
    /* SPNG_FMT_PNG is a special value that matches the format in ihdr */
    fmt = SPNG_FMT_PNG;

    /* SPNG_ENCODE_FINALIZE will finalize the PNG with the end-of-file marker */
    ret = spng_encode_image(ctx, image, length, fmt, SPNG_ENCODE_FINALIZE);

    if (ret)
    {
        printf("spng_encode_image() error: %s\n", spng_strerror(ret));
        goto encode_error;
    }

    size_t png_size;
    void *png_buf = NULL;

    /* Get the internal buffer of the finished PNG */
    png_buf = spng_get_png_buffer(ctx, &png_size, &ret);

    if (png_buf == NULL)
    {
        printf("spng_get_png_buffer() error: %s\n", spng_strerror(ret));
    }

    /* User owns the buffer after a successful call */
    free(png_buf);

    encode_error:

    spng_ctx_free(ctx);

    return ret;
}


const char *color_type_str(enum spng_color_type colorType)
{
    switch (colorType)
    {
        case SPNG_COLOR_TYPE_GRAYSCALE: return "greyscale";
        case SPNG_COLOR_TYPE_TRUECOLOR: return "truecolor";
        case SPNG_COLOR_TYPE_INDEXED: return "indexed color";
        case SPNG_COLOR_TYPE_GRAYSCALE_ALPHA: return "greyscale with alpha";
        case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA: return "trucolor with alpha";
        default:return "(invalid)";
    }
}


int image_main(int argc, char **argv)
{
    FILE *png;
    int ret = 0;
    spng_ctx  *ctx = NULL;
    unsigned char *image = NULL;

    if (argc < 2)
    {
        printf("no input file\n");
        goto error;
    }

    png = fopen(argv[1], "rb");

    if (png == NULL)
    {
        printf("error opening input file %s\n", argv[1]);
        goto error;
    }

    ctx = spng_ctx_new(0);

    if (ctx == NULL)
    {
        printf("spng_ctx_new() failed\n");
        goto error;
    }

    /* Ignore and don't calculate chunk CRC's */
    spng_set_crc_action(ctx, SPNG_CRC_USE, SPNG_CRC_USE);

    /* Set memory usage limits for storing standard and unknown chunks,
      this is important when reading untrusted files! */
    size_t limit = 1024 * 1024 * 64;
    spng_set_chunk_limits(ctx, limit, limit);

    /* Set source PNG */
    spng_set_png_file(ctx, png);    /* or _buffer(), _stream() */

    struct spng_ihdr ihdr;
    ret = spng_get_ihdr(ctx, &ihdr);

    if (ret)
    {
        printf("spng_get_ihdr() error: %s\n", spng_strerror(ret));
        goto error;
    }

    const char * color_name = color_type_str(ihdr.color_type);

    printf("width: %u\n"
                    "height: %u\n"
                    "bit depth: %u\n"
                    "color type: %u - %s\n",
                    ihdr.width, ihdr.height, ihdr.bit_depth, ihdr.color_type, color_name);

    printf("compression method: %u\n"
                    "filter method: %u\n"
                    "interlace method: %u\n",
                    ihdr.compression_method, ihdr.filter_method, ihdr.interlace_method);

    struct spng_plte plte = {0};
    ret = spng_get_plte(ctx, &plte);

    if (ret && ret != SPNG_ECHUNKAVAIL)
    {
        printf("spng_get_plte() error: %s\n", spng_strerror(ret));
        goto error;
    }

    if (!ret) printf("pallete entries: %u\n", plte.n_entries);


    size_t image_size, image_width;

    error:

    spng_ctx_free(ctx);
    free(image);

    return ret;
}


