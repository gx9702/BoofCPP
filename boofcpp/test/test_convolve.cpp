#include "gtest/gtest.h"
#include "convolve.h"
#include "image_misc_ops.h"
#include "testing_utils.h"

using namespace std;
using namespace boofcv;


U8 horizontal_norm(uint32_t x , uint32_t y , const Kernel1D<S32>& kernel , const Gray<U8>& image )
{
    S32 total = 0;
    S32 weight = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        if( image.isInBounds(x+i-kernel.offset,y)) {
            S32 w = kernel.at(i);
            U8 v = image.at(x+i-kernel.offset,y);

            total += w*v;
            weight += w;
        }
    }

    return static_cast<U8>((total + weight/2)/weight);
}

TEST(ConvolveNormalizedNaive, horizontal_1D_int) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<S32> kernel(4,{1,2,3,4,5,6});

    Gray<U8> input(15,16);
    ImageMiscOps::fill_uniform(input, (U8)0, (U8)50, gen);
    Gray<U8> output(15,16);

    ConvolveNormalizedNaive::horizontal(kernel,input,output);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            S32 expected = horizontal_norm(x,y,kernel,input);
            S32 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}

F32 horizontal_norm(uint32_t x , uint32_t y , const Kernel1D<F32>& kernel , const Gray<F32>& image )
{
    F32 total = 0;
    F32 weight = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        if( image.isInBounds(x+i-kernel.offset,y)) {
            F32 w = kernel.at(i);
            F32 v = image.at(x+i-kernel.offset,y);

            total += w*v;
            weight += w;
        }
    }

    return total/weight;
}

TEST(ConvolveNormalizedNaive, horizontal_1D_float) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<F32> kernel(4,{1,2,3,4,5,6});

    Gray<F32> input(15,16);
    ImageMiscOps::fill_uniform(input, (F32)0, (F32)50, gen);
    Gray<F32> output(15,16);

    ConvolveNormalizedNaive::horizontal(kernel,input,output);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            F32 expected = horizontal_norm(x,y,kernel,input);
            F32 found = output.at(x,y);
            ASSERT_FLOAT_EQ(expected,found);
        }
    }
}

U8 vertical_norm(uint32_t x , uint32_t y , const Kernel1D<S32>& kernel , const Gray<U8>& image )
{
    S32 total = 0;
    S32 weight = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        if( image.isInBounds(x,y+i-kernel.offset)) {
            S32 w = kernel.at(i);
            S32 v = image.at(x,y+i-kernel.offset);

            total += w*v;
            weight += w;
        }
    }

    return static_cast<U8>((total + weight/2)/weight);
}

TEST(ConvolveNormalizedNaive, vertical_1D_int) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<S32> kernel(4,{1,2,3,4,5,6});

    Gray<U8> input(15,16);
    ImageMiscOps::fill_uniform(input, (U8)0, (U8)50,gen);
    Gray<U8> output(15,16);

    ConvolveNormalizedNaive::vertical(kernel, input, output);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            U8 expected = vertical_norm(x,y,kernel,input);
            U8 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}


F32 vertical_norm(uint32_t x , uint32_t y , const Kernel1D<F32>& kernel , const Gray<F32>& image )
{
    F32 total = 0;
    F32 weight = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        if( image.isInBounds(x,y+i-kernel.offset)) {
            F32 w = kernel.at(i);
            F32 v = image.at(x,y+i-kernel.offset);

            total += w*v;
            weight += w;
        }
    }

    return total/weight;
}

TEST(ConvolveNormalizedNaive, vertical_1D_float) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<F32> kernel(4,{1,2,3,4,5,6});

    Gray<F32> input(15,16);
    ImageMiscOps::fill_uniform(input, (F32)0, (F32)50,gen);
    Gray<F32> output(15,16);

    ConvolveNormalizedNaive::vertical(kernel, input, output);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            F32 expected = vertical_norm(x,y,kernel,input);
            F32 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}

U8 horizontal(uint32_t x , uint32_t y , const Kernel1D<S32>& kernel , const ImageBorder<U8>& image )
{
    S32 total = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        // going overboard on typecasting to make sure it's signed
        int32_t xx = (int32_t)(x+i)-(int32_t)kernel.offset;

        S32 w = kernel.at(i);
        U8 v = image.get(xx,y);

        total += w*v;
    }

    return (U8)total;
}

TEST(ConvolveNaive, horizontal_1D_int) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<S32> kernel(4,{1,2,3,4,5,6});

    Gray<U8> input(15,16);
    ImageMiscOps::fill_uniform(input, (U8)0, (U8)50, gen);
    Gray<U8> output(15,16);

    ImageBorderValue<U8> border(5);
    border.setImage(input);

    ConvolveNaive::horizontal(kernel,border,output);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            U8 expected = horizontal(x,y,kernel,border);
            U8 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}

U8 vertical(uint32_t x , uint32_t y , const Kernel1D<S32>& kernel , const ImageBorder<U8>& image )
{
    S32 total = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        // going overboard on typecasting to make sure it's signed
        int32_t yy = (int32_t)(y+i)-(int32_t)kernel.offset;

        S32 w = kernel.at(i);
        U8 v = image.get(x,yy);

        total += w*v;
    }

    return (U8)total;
}

TEST(ConvolveNaive, vertical_1D_int) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<S32> kernel(4,{1,2,3,4,5,6});

    Gray<U8> input(15,16);
    ImageMiscOps::fill_uniform(input, (U8)0, (U8)50, gen);
    Gray<U8> output(15,16);

    ImageBorderValue<U8> border(5);
    border.setImage(input);

    ConvolveNaive::vertical(kernel,border,output);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            U8 expected = vertical(x,y,kernel,border);
            U8 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}

U8 horizontal_divide(uint32_t x , uint32_t y , const Kernel1D<S32>& kernel , const ImageBorder<U8>& image, S32 divisor )
{
    S32 total = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        // going overboard on typecasting to make sure it's signed
        int32_t xx = (int32_t)(x+i)-(int32_t)kernel.offset;

        S32 w = kernel.at(i);
        U8 v = image.get(xx,y);

        total += w*v;
    }

    return static_cast<U8>((total+divisor/2)/divisor);
}

TEST(ConvolveNaive, horizontal_divide_1D_int) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<S32> kernel(4,{1,2,3,4,5,6});

    Gray<U8> input(15,16);
    ImageMiscOps::fill_uniform(input, (U8)0, (U8)50, gen);
    Gray<U8> output(15,16);

    ImageBorderValue<U8> border(5);
    border.setImage(input);

    S32 divisor = 16;

    ConvolveNaive::horizontal(kernel,border,output,divisor);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            U8 expected = horizontal_divide(x,y,kernel,border,divisor);
            U8 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}

F32 horizontal_divide(uint32_t x , uint32_t y , const Kernel1D<F32>& kernel , const ImageBorder<F32>& image, F32 divisor )
{
    F32 total = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        // going overboard on typecasting to make sure it's signed
        int32_t xx = (int32_t)(x+i)-(int32_t)kernel.offset;

        F32 w = kernel.at(i);
        F32 v = image.get(xx,y);

        total += w*v;
    }

    return total/divisor;
}

TEST(ConvolveNaive, horizontal_divide_1D_float) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<F32> kernel(4,{1,2,3,4,5,6});

    Gray<F32> input(15,16);
    ImageMiscOps::fill_uniform(input, (F32)0, (F32)50, gen);
    Gray<F32> output(15,16);

    ImageBorderValue<F32> border(5);
    border.setImage(input);

    F32 divisor = 14.5f;

    ConvolveNaive::horizontal(kernel,border,output,divisor);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            F32 expected = horizontal_divide(x,y,kernel,border,divisor);
            F32 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}

U8 vertical_divide(uint32_t x , uint32_t y , const Kernel1D<S32>& kernel , const ImageBorder<U8>& image, S32 divisor )
{
    S32 total = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        // going overboard on typecasting to make sure it's signed
        int32_t yy = (int32_t)(y+i)-(int32_t)kernel.offset;

        S32 w = kernel.at(i);
        U8 v = image.get(x,yy);

        total += w*v;
    }

    return static_cast<U8>((total+divisor/2)/divisor);
}

TEST(ConvolveNaive, vertical_divide_1D_int) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<S32> kernel(4,{1,2,3,4,5,6});

    Gray<U8> input(15,16);
    ImageMiscOps::fill_uniform(input, (U8)0, (U8)50, gen);
    Gray<U8> output(15,16);

    ImageBorderValue<U8> border(5);
    border.setImage(input);

    S32 divisor = 16;

    ConvolveNaive::vertical(kernel,border,output,divisor);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            U8 expected = vertical_divide(x,y,kernel,border,divisor);
            U8 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}

F32 vertical_divide(uint32_t x , uint32_t y , const Kernel1D<F32>& kernel , const ImageBorder<F32>& image, F32 divisor )
{
    F32 total = 0;

    for (uint32_t i = 0; i < kernel.width; i++) {
        // going overboard on typecasting to make sure it's signed
        int32_t yy = (int32_t)(y+i)-(int32_t)kernel.offset;

        F32 w = kernel.at(i);
        F32 v = image.get(x,yy);

        total += w*v;
    }

    return total/divisor;
}

TEST(ConvolveNaive, vertical_divide_1D_float) {
    std::mt19937 gen(0xBEEF);
    Kernel1D<F32> kernel(4,{1,2,3,4,5,6});

    Gray<F32> input(15,16);
    ImageMiscOps::fill_uniform(input, (F32)0, (F32)50, gen);
    Gray<F32> output(15,16);

    ImageBorderValue<F32> border(5);
    border.setImage(input);

    F32 divisor = 16.5f;

    ConvolveNaive::vertical(kernel,border,output,divisor);

    for (uint32_t y = 0; y < output.height; y++) {
        for (uint32_t x = 0; x < output.width; x++) {
            F32 expected = vertical_divide(x,y,kernel,border,divisor);
            F32 found = output.at(x,y);
            ASSERT_EQ(expected,found);
        }
    }
}

template<class E>
class CompareToNaive {
public:
    typedef typename TypeInfo<E>::signed_type signed_type;

    Gray<E> input,found,expected;
    Kernel1D<signed_type> kernel;

    std::mt19937 gen;

    uint32_t borderX0,borderX1;
    uint32_t borderY0,borderY1;

    CompareToNaive() : gen(0xBEEF) {

    }

    void setImageSize( uint32_t width , uint32_t height ) {

        input.reshape(width,height);
        ImageMiscOps::fill_uniform(input, (E)0, (E)50,gen);
        found.reshape(width,height);
        expected.reshape(width,height);
    }

    void setKernel( uint32_t width , uint32_t offset , bool has_negative=false) {
        kernel.reshape(width,offset);
        if( has_negative )
            KernelOps::fill_uniform(kernel,(signed_type)-10,(signed_type)10,gen);
        else
            KernelOps::fill_uniform(kernel,(signed_type)0,(signed_type)10,gen);
    }

    void horizontal_norm_border() {
        ConvolveNormalizedNaive::horizontal(kernel,input,expected);
        ConvolveNormalized_JustBorder::horizontal(kernel,input,found);

        borderY0=borderY1=0;
        borderX0 = kernel.offset;
        borderX1 = kernel.width-1-kernel.offset;

        checkResults_border();
    }

    void vertical_norm_border() {
        ConvolveNormalizedNaive::vertical(kernel,input,expected);
        ConvolveNormalized_JustBorder::vertical(kernel,input,found);

        borderX0=borderX1=0;
        borderY0 = kernel.offset;
        borderY1 = kernel.width-1-kernel.offset;

        checkResults_border();
    }

    void horizontal_div_inner() {
        ImageBorderValue<E> border(0);
        border.setImage(input);

        signed_type kernel_sum = kernel.sum();
        ConvolveNaive::horizontal(kernel,border,expected,kernel_sum);
        ConvolveImage_Inner::horizontal(kernel,input,found,kernel_sum);

        borderY0=borderY1=0;
        borderX0 = kernel.offset;
        borderX1 = kernel.width-1-kernel.offset;

        checkResults_inner();
    }

    void vertical_div_inner() {
        ImageBorderValue<E> border(0);
        border.setImage(input);

        signed_type kernel_sum = kernel.sum();
        ConvolveNaive::vertical(kernel,border,expected,kernel_sum);
        ConvolveImage_Inner::vertical(kernel,input,found,kernel_sum);

        borderX0=borderX1=0;
        borderY0 = kernel.offset;
        borderY1 = kernel.width-1-kernel.offset;

        checkResults_inner();
    }

    void horizontal_inner() {
        ImageBorderValue<E> border(0);
        border.setImage(input);

        ConvolveNaive::horizontal(kernel,border,expected);
        ConvolveImage_Inner::horizontal(kernel,input,found);

        borderY0=borderY1=0;
        borderX0 = kernel.offset;
        borderX1 = kernel.width-1-kernel.offset;

        checkResults_inner();
    }

    void vertical_inner() {
        ImageBorderValue<E> border(0);
        border.setImage(input);

        ConvolveNaive::vertical(kernel,border,expected);
        ConvolveImage_Inner::vertical(kernel,input,found);

        borderX0=borderX1=0;
        borderY0 = kernel.offset;
        borderY1 = kernel.width-1-kernel.offset;

        checkResults_inner();
    }

    void normalized_horizontal() {
        ConvolveNormalizedNaive::horizontal(kernel,input,expected);
        ConvolveNormalized::horizontal(kernel,input,found);

        check_equals(expected,found);
    }

    void normalized_vertical() {
        ConvolveNormalizedNaive::vertical(kernel,input,expected);
        ConvolveNormalized::vertical(kernel,input,found);

        check_equals(expected,found);
    }

    void normalized_convolve_1D() {
        Gray<E> workspace(input.width,input.height);
        ConvolveNormalizedNaive::horizontal(kernel,input,workspace);
        ConvolveNormalizedNaive::vertical(kernel,workspace,expected);
        ConvolveNormalized::convolve(kernel,input,found,workspace);

        check_equals(expected,found);
    }

    void checkResults_border() {
        check_equals_border(expected,found,borderX0,borderX1,borderY0,borderY1);
    }

    void checkResults_inner() {
        check_equals_inner(expected,found,borderX0,borderX1,borderY0,borderY1);
    }

};

TEST(ConvolveNormalized_JustBorder, horizontal_U8) {
    CompareToNaive<U8> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.horizontal_norm_border();
        compare.setKernel(5,2);
        compare.horizontal_norm_border();
        compare.setKernel(7,3);
        compare.horizontal_norm_border();
        compare.setKernel(7,1);
        compare.horizontal_norm_border();
        compare.setKernel(7,4);
        compare.horizontal_norm_border();
    }
}

TEST(ConvolveNormalized_JustBorder, horizontal_F32) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.horizontal_norm_border();
        compare.setKernel(5,2);
        compare.horizontal_norm_border();
        compare.setKernel(7,3);
        compare.horizontal_norm_border();
        compare.setKernel(7,1);
        compare.horizontal_norm_border();
        compare.setKernel(7,4);
        compare.horizontal_norm_border();
    }
}

TEST(ConvolveNormalized_JustBorder, vertical_U8) {
    CompareToNaive<U8> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.vertical_norm_border();
        compare.setKernel(5,2);
        compare.vertical_norm_border();
        compare.setKernel(7,3);
        compare.vertical_norm_border();
        compare.setKernel(7,1);
        compare.vertical_norm_border();
        compare.setKernel(7,4);
        compare.vertical_norm_border();
    }
}

TEST(ConvolveNormalized_JustBorder, vertical_F32) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.vertical_norm_border();
        compare.setKernel(5,2);
        compare.vertical_norm_border();
        compare.setKernel(7,3);
        compare.vertical_norm_border();
        compare.setKernel(7,1);
        compare.vertical_norm_border();
        compare.setKernel(7,4);
        compare.vertical_norm_border();
    }
}

TEST(ConvolveImage_Inner, horizontal_div_U8) {
    CompareToNaive<U8> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.horizontal_div_inner();
        compare.setKernel(5,2);
        compare.horizontal_div_inner();
        compare.setKernel(7,3);
        compare.horizontal_div_inner();
        compare.setKernel(7,1);
        compare.horizontal_div_inner();
        compare.setKernel(7,4);
        compare.horizontal_div_inner();
    }
}

TEST(ConvolveImage_Inner, horizontal_div_F32) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.horizontal_div_inner();
        compare.setKernel(5,2);
        compare.horizontal_div_inner();
        compare.setKernel(7,3);
        compare.horizontal_div_inner();
        compare.setKernel(7,1);
        compare.horizontal_div_inner();
        compare.setKernel(7,4);
        compare.horizontal_div_inner();
    }
}

TEST(ConvolveImage_Inner, vertical_div_U8) {
    CompareToNaive<U8> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.vertical_div_inner();
        compare.setKernel(5,2);
        compare.vertical_div_inner();
        compare.setKernel(7,3);
        compare.vertical_div_inner();
        compare.setKernel(7,1);
        compare.vertical_div_inner();
        compare.setKernel(7,4);
        compare.vertical_div_inner();
    }
}

TEST(ConvolveImage_Inner, vertical_div_F32) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.vertical_div_inner();
        compare.setKernel(5,2);
        compare.vertical_div_inner();
        compare.setKernel(7,3);
        compare.vertical_div_inner();
        compare.setKernel(7,1);
        compare.vertical_div_inner();
        compare.setKernel(7,4);
        compare.vertical_div_inner();
    }
}

TEST(ConvolveImage_Inner, horizontal_U8) {
    CompareToNaive<U8> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.horizontal_inner();
        compare.setKernel(5,2);
        compare.horizontal_inner();
        compare.setKernel(7,3);
        compare.horizontal_inner();
        compare.setKernel(7,1);
        compare.horizontal_inner();
        compare.setKernel(7,4);
        compare.horizontal_inner();
    }
}

TEST(ConvolveImage_Inner, horizontal_F32) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.horizontal_inner();
        compare.setKernel(5,2);
        compare.horizontal_inner();
        compare.setKernel(7,3);
        compare.horizontal_inner();
        compare.setKernel(7,1);
        compare.horizontal_inner();
        compare.setKernel(7,4);
        compare.horizontal_inner();
    }
}

TEST(ConvolveImage_Inner, vertical_U8) {
    CompareToNaive<U8> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.vertical_inner();
        compare.setKernel(5,2);
        compare.vertical_inner();
        compare.setKernel(7,3);
        compare.vertical_inner();
        compare.setKernel(7,1);
        compare.vertical_inner();
        compare.setKernel(7,4);
        compare.vertical_inner();
    }
}

TEST(ConvolveImage_Inner, vertical_F32) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.vertical_inner();
        compare.setKernel(5,2);
        compare.vertical_inner();
        compare.setKernel(7,3);
        compare.vertical_inner();
        compare.setKernel(7,1);
        compare.vertical_inner();
        compare.setKernel(7,4);
        compare.vertical_inner();
    }
}

TEST(ConvolveNormalized, horizontal_U8) {
    CompareToNaive<U8> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.normalized_horizontal();
        compare.setKernel(5,2);
        compare.normalized_horizontal();
        compare.setKernel(7,3);
        compare.normalized_horizontal();
        compare.setKernel(7,1);
        compare.normalized_horizontal();
        compare.setKernel(7,4);
        compare.normalized_horizontal();
    }
}

TEST(ConvolveNormalized, horizontal_F32) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.normalized_horizontal();
        compare.setKernel(5,2);
        compare.normalized_horizontal();
        compare.setKernel(7,3);
        compare.normalized_horizontal();
        compare.setKernel(7,1);
        compare.normalized_horizontal();
        compare.setKernel(7,4);
        compare.normalized_horizontal();
    }
}

TEST(ConvolveNormalized, vertical_U8) {
    CompareToNaive<U8> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.normalized_vertical();
        compare.setKernel(5,2);
        compare.normalized_vertical();
        compare.setKernel(7,3);
        compare.normalized_vertical();
        compare.setKernel(7,1);
        compare.normalized_vertical();
        compare.setKernel(7,4);
        compare.normalized_vertical();
    }
}

TEST(ConvolveNormalized, vertical_F32) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.normalized_vertical();
        compare.setKernel(5,2);
        compare.normalized_vertical();
        compare.setKernel(7,3);
        compare.normalized_vertical();
        compare.setKernel(7,1);
        compare.normalized_vertical();
        compare.setKernel(7,4);
        compare.normalized_vertical();
    }
}

TEST(ConvolveNormalized, convolve_with1D) {
    CompareToNaive<F32> compare;

    for( uint32_t i = 0; i < 2; i++ ) {
        uint32_t w = 15+i;
        uint32_t h = 20+i;

        compare.setImageSize(w,h);
        compare.setKernel(3,1);
        compare.normalized_convolve_1D();
        compare.setKernel(5,2);
        compare.normalized_convolve_1D();
        compare.setKernel(7,3);
        compare.normalized_convolve_1D();
        compare.setKernel(7,1);
        compare.normalized_convolve_1D();
        compare.setKernel(7,4);
        compare.normalized_convolve_1D();
    }
}