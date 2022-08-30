/*
main.cpp
CSPB 1300 Image Processing Application

PLEASE FILL OUT THIS SECTION PRIOR TO SUBMISSION

- Your name:
    <Cyro Estevão Freire de Lima>

- All project requirements fully met? (YES or NO):
    <YES>

- If no, please explain what you could not get to work:
    <ANSWER>

- Did you do any optional enhancements? If so, please explain:
    <Yes, I tried to do the sepia tone althought the result didn't fully worked.>
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>
#include<algorithm>
#include<stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>


using namespace std;

//***************************************************************************************************//
//                                DO NOT MODIFY THE SECTION BELOW                                    //
//***************************************************************************************************//

// Pixel structure
struct Pixel
{
    // Red, green, blue color values
    int red;
    int green;
    int blue;
};

/**
 * Gets an integer from a binary stream.
 * Helper function for read_image()
 * @param stream the stream
 * @param offset the offset at which to read the integer
 * @param bytes  the number of bytes to read
 * @return the integer starting at the given offset
 */ 
int get_int(fstream& stream, int offset, int bytes)
{
    stream.seekg(offset);
    int result = 0;
    int base = 1;
    for (int i = 0; i < bytes; i++)
    {   
        result = result + stream.get() * base;
        base = base * 256;
    }
    return result;
}

/**
 * Reads the BMP image specified and returns the resulting image as a vector
 * @param filename BMP image filename
 * @return the image as a vector of vector of Pixels
 */
vector<vector<Pixel>> read_image(string filename)
{
    // Open the binary file
    fstream stream;
    stream.open(filename, ios::in | ios::binary);

    // Get the image properties
    int file_size = get_int(stream, 2, 4);
    int start = get_int(stream, 10, 4);
    int width = get_int(stream, 18, 4);
    int height = get_int(stream, 22, 4);
    int bits_per_pixel = get_int(stream, 28, 2);

    // Scan lines must occupy multiples of four bytes
    int scanline_size = width * (bits_per_pixel / 8);
    int padding = 0;
    if (scanline_size % 4 != 0)
    {
        padding = 4 - scanline_size % 4;
    }

    // Return empty vector if this is not a valid image
    if (file_size != start + (scanline_size + padding) * height)
    {
        return {};
    }

    // Create a vector the size of the input image
    vector<vector<Pixel>> image(height, vector<Pixel> (width));

    int pos = start;
    // For each row, starting from the last row to the first
    // Note: BMP files store pixels from bottom to top
    for (int i = height - 1; i >= 0; i--)
    {
        // For each column
        for (int j = 0; j < width; j++)
        {
            // Go to the pixel position
            stream.seekg(pos);

            // Save the pixel values to the image vector
            // Note: BMP files store pixels in blue, green, red order
            image[i][j].blue = stream.get();
            image[i][j].green = stream.get();
            image[i][j].red = stream.get();

            // We are ignoring the alpha channel if there is one

            // Advance the position to the next pixel
            pos = pos + (bits_per_pixel / 8);
        }

        // Skip the padding at the end of each row
        stream.seekg(padding, ios::cur);
        pos = pos + padding;
    }

    // Close the stream and return the image vector
    stream.close();
    return image;
}

/**
 * Sets a value to the char array starting at the offset using the size
 * specified by the bytes.
 * This is a helper function for write_image()
 * @param arr    Array to set values for
 * @param offset Starting index offset
 * @param bytes  Number of bytes to set
 * @param value  Value to set
 * @return nothing
 */
void set_bytes(unsigned char arr[], int offset, int bytes, int value)
{
    for (int i = 0; i < bytes; i++)
    {
        arr[offset+i] = (unsigned char)(value>>(i*8));
    }
}

/**
 * Write the input image to a BMP file name specified
 * @param filename The BMP file name to save the image to
 * @param image    The input image to save
 * @return True if successful and false otherwise
 */
bool write_image(string filename, const vector<vector<Pixel>>& image)
{
    // Get the image width and height in pixels
    int width_pixels = image[0].size();
    int height_pixels = image.size();

    // Calculate the width in bytes incorporating padding (4 byte alignment)
    int width_bytes = width_pixels * 3;
    int padding_bytes = 0;
    padding_bytes = (4 - width_bytes % 4) % 4;
    width_bytes = width_bytes + padding_bytes;

    // Pixel array size in bytes, including padding
    int array_bytes = width_bytes * height_pixels;

    // Open a file stream for writing to a binary file
    fstream stream;
    stream.open(filename, ios::out | ios::binary);

    // If there was a problem opening the file, return false
    if (!stream.is_open())
    {
        return false;
    }

    // Create the BMP and DIB Headers
    const int BMP_HEADER_SIZE = 14;
    const int DIB_HEADER_SIZE = 40;
    unsigned char bmp_header[BMP_HEADER_SIZE] = {0};
    unsigned char dib_header[DIB_HEADER_SIZE] = {0};

    // BMP Header
    set_bytes(bmp_header,  0, 1, 'B');              // ID field
    set_bytes(bmp_header,  1, 1, 'M');              // ID field
    set_bytes(bmp_header,  2, 4, BMP_HEADER_SIZE+DIB_HEADER_SIZE+array_bytes); // Size of BMP file
    set_bytes(bmp_header,  6, 2, 0);                // Reserved
    set_bytes(bmp_header,  8, 2, 0);                // Reserved
    set_bytes(bmp_header, 10, 4, BMP_HEADER_SIZE+DIB_HEADER_SIZE); // Pixel array offset

    // DIB Header
    set_bytes(dib_header,  0, 4, DIB_HEADER_SIZE);  // DIB header size
    set_bytes(dib_header,  4, 4, width_pixels);     // Width of bitmap in pixels
    set_bytes(dib_header,  8, 4, height_pixels);    // Height of bitmap in pixels
    set_bytes(dib_header, 12, 2, 1);                // Number of color planes
    set_bytes(dib_header, 14, 2, 24);               // Number of bits per pixel
    set_bytes(dib_header, 16, 4, 0);                // Compression method (0=BI_RGB)
    set_bytes(dib_header, 20, 4, array_bytes);      // Size of raw bitmap data (including padding)                     
    set_bytes(dib_header, 24, 4, 2835);             // Print resolution of image (2835 pixels/meter)
    set_bytes(dib_header, 28, 4, 2835);             // Print resolution of image (2835 pixels/meter)
    set_bytes(dib_header, 32, 4, 0);                // Number of colors in palette
    set_bytes(dib_header, 36, 4, 0);                // Number of important colors

    // Write the BMP and DIB Headers to the file
    stream.write((char*)bmp_header, sizeof(bmp_header));
    stream.write((char*)dib_header, sizeof(dib_header));

    // Initialize pixel and padding
    unsigned char pixel[3] = {0};
    unsigned char padding[3] = {0};

    // Pixel Array (Left to right, bottom to top, with padding)
    for (int h = height_pixels - 1; h >= 0; h--)
    {
        for (int w = 0; w < width_pixels; w++)
        {
            // Write the pixel (Blue, Green, Red)
            pixel[0] = image[h][w].blue;
            pixel[1] = image[h][w].green;
            pixel[2] = image[h][w].red;
            stream.write((char*)pixel, 3);
        }
        // Write the padding bytes
        stream.write((char *)padding, padding_bytes);
    }

    // Close the stream and return true
    stream.close();
    return true;
}

//***************************************************************************************************//
//                                DO NOT MODIFY THE SECTION ABOVE                                    //
//***************************************************************************************************//


//
// YOUR FUNCTION DEFINITIONS HERE
//
vector<vector<Pixel>> process_1(const vector<vector<Pixel>>& image)
{
    
    double r= image.size(); //height 
    int c= image[0].size();//width
    // Get the number of rows/columns from the input 2D vector

    vector<vector<Pixel>> vec2(r, vector<Pixel> (c));
 // Define a new 2D vector the same size as the input 2D vector

    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            int blue = image[row][col].blue;  
            int red = image[row][col].red;
            int green = image[row][col].green; //Getting RGB
            int distance = sqrt((col-c/2)*(col-c/2) + (row-r/2)*(row-r/2));//processing
            double scaling_factor = (r-distance)/r;//scaling factor equation
            int newred = red*scaling_factor;
            int newgreen = green*scaling_factor;
            int newblue = blue*scaling_factor;  //Applied new RGB
            vec2[row][col].green=newgreen;
            vec2[row][col].red=newred;
            vec2[row][col].blue=newblue; //Process new image
            
            

        }
    }
    return vec2;
    
    // Iterate through the pixels of the input 2D vector (nested loop)

        // Get the color values for a single pixel in the input 2D vector

        // Perform the operation on the color values (refer to Runestone for this)

        // Save the new color values to the corresponding pixel in the new 2D vector

    // Return the new 2D vector
}
///////////////////////////////////////////////////////////////////

vector<vector<Pixel>> process_2(const vector<vector<Pixel>>& image, double scaling_factor)
{
    
    double r= image.size(); //height 
    int c= image[0].size();//width
    vector<vector<Pixel>> vec3(r, vector<Pixel> (c));
    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            int blue_value = image[row][col].blue;  
            int red_value = image[row][col].red;
            int green_value = image[row][col].green;
            
            double average_value= (red_value+green_value+blue_value)/3.0; //average RGB value
            
            if (average_value >= 170)//applying claredon depending on the value of the pixel
            {
                double newred = double(255 - (255 - red_value)*scaling_factor); ///equation to lightened the image
                double newgreen = double(255 - (255 - green_value)*scaling_factor);
                double newblue =  double(255 - (255 - blue_value)*scaling_factor);
                
            vec3[row][col].green= newgreen; //making a new image
            vec3[row][col].red= newred;
            vec3[row][col].blue= newblue;
            }
            else if (average_value < 90)
            {
                double newred = red_value*scaling_factor; 
                double newgreen = green_value*scaling_factor;
                double newblue =  blue_value*scaling_factor;
                
                vec3[row][col].green= newgreen;
                vec3[row][col].red= newred;
                vec3[row][col].blue= newblue;
            }
            else
            {
                
                double newred = red_value;
                double newgreen = green_value;
                double newblue =  blue_value;
                
                vec3[row][col].green= newgreen;
                vec3[row][col].red= newred;
                vec3[row][col].blue= newblue;
            }
            
            
            
            

        }
    }
    return vec3;
}
///////////////////////////////////////////////////////////////

vector<vector<Pixel>> process_3(const vector<vector<Pixel>>& image)
{
  
    
    double r= image.size(); //height 
    int c= image[0].size();//width
    // Get the number of rows/columns from the input 2D vector

    vector<vector<Pixel>> vec4(r, vector<Pixel> (c));
 // Define a new 2D vector the same size as the input 2D vector

    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            int blue = image[row][col].blue;  
            int red = image[row][col].red;
            int green = image[row][col].green;
            int gray=(blue+red+green)/3;/// equation to make the pixels gray
                
            
            int newred = gray;///applying gray to the RGB
            int newgreen = gray;
            int newblue = gray;
            vec4[row][col].green=newgreen; //aplying the new pixels
            vec4[row][col].red=newred;
            vec4[row][col].blue=newblue;
            
            

        }
    }
    return vec4;  
}
///////////////////////////////////////////////////////////////
vector<vector<Pixel>> process_4(const vector<vector<Pixel>>& image)
{
    double r= image.size(); //height 
    int c= image[0].size();//width
     

    vector<vector<Pixel>> vec5(c, vector<Pixel> (r));

    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            int blue = image[row][col].blue;  
            int red = image[row][col].red;
            int green = image[row][col].green;
            
            vec5[col][((r-1)-row)].blue=blue;//Applying rotated RGB
            vec5[col][((r-1)-row)].red=red;
            vec5[col][((r-1)-row)].green=green;
            
            

    
            

        }
    }
    
    
    return vec5;
    
}
//////////////////////////////////////////////////////////////
vector<vector<Pixel>> process_5(const vector<vector<Pixel>>& image, int number)

{
    double r= image.size(); //height 
    int c= image[0].size();//width
     

    vector<vector<Pixel>> vec6(c, vector<Pixel> (r));

    int angle = (number*90);
    
    if (angle%90 != 0)
        {cout<<"angle must be a multiple of 90 degrees."<<endl;}
    
    else if (angle%360 == 0)
        {return image;}
    
    else if (angle%360 == 90) //rotate 90
        {vector<vector<Pixel>> vec6(c, vector<Pixel> (r));

        return process_4(image);} // Uses process_4 to rotate images
    
    else if (angle%360 == 180) // rotate 180
        {vector<vector<Pixel>> vec6(r, vector<Pixel> (c));

        return vec6 =process_4(process_4(image));} //Nested process_4
    
    else //rotate 270
        {vector<vector<Pixel>> vec6(c, vector<Pixel> (r));
        return vec6=process_4(process_4(process_4(image)));}
    
    return vec6;
    
    
}
/////////******************************//////////////////////
vector<vector<Pixel>> process_6(const vector<vector<Pixel>>& image, int x_scale, int y_scale)
{
 
    
    int r= image.size(); //height 
    int c= image[0].size();//width
    int newheight=y_scale*r;
    int newwidth =x_scale*c;

    vector<vector<Pixel>> vec7(newheight, vector<Pixel> (newwidth));

    for (int row = 0; row < newheight; row++)
    {
        for (int col = 0; col < newwidth; col++)
        {
            int blue = image[row/y_scale][col/x_scale].blue;//Getting row and column divided by the new scale  
            int red = image[row/y_scale][col/x_scale].red;
            int green = image[row/y_scale][col/x_scale].green;
            
            vec7[row][col].blue=blue; //applying the new parameters
            vec7[row][col].red=red;
            vec7[row][col].green=green;
           
            
        
        }
    }
    return vec7;  
}
////////////////////////////////////////////////////////////////
vector<vector<Pixel>> process_7(const vector<vector<Pixel>>& image)
{
    double r= image.size(); //height 
    int c= image[0].size();//width

    vector<vector<Pixel>> vec8(r, vector<Pixel> (c));

    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            int blue = image[row][col].blue;  
            int red = image[row][col].red;
            int green = image[row][col].green;
            int gray_value = (red + green + blue)/3; //average of the RGB making a gray value
                
            if (gray_value >= 255/2)  //increasing the pixel value
                {
                int newred = 255;
                int newgreen = 255;
                int newblue = 255;
                
                vec8[row][col].green=newgreen;
                vec8[row][col].red=newred;
                vec8[row][col].blue=newblue;
                }
            else //decreasing pixel value creating contrast
                {
                int newred = 0;
                int newgreen = 0;
                int newblue = 0;
                vec8[row][col].green=newgreen;
                vec8[row][col].red=newred;
                vec8[row][col].blue=newblue;
                }
            
            
            
            

        }
    }
    return vec8; 
}
/////////////////////////////////////////////////////////////////
vector<vector<Pixel>> process_8(const vector<vector<Pixel>>& image, double scaling_factor)
{
     double r= image.size(); //height 
    int c= image[0].size();//width

    vector<vector<Pixel>> vec9(r, vector<Pixel> (c));

    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            double blue_value = image[row][col].blue;  
            double red_value = image[row][col].red;
            double green_value = image[row][col].green;
            int newred = (255 - (255 - red_value)*scaling_factor); //lightning all the pixels (RGB)
            int newgreen = (255 - (255 - green_value)*scaling_factor);
            int newblue = (255 - (255 - blue_value)*scaling_factor);
                
            
            
            vec9[row][col].green=newgreen;//creating new image
            vec9[row][col].red=newred;
            vec9[row][col].blue=newblue;
            
            

        }
    }
    return vec9;   
}
////////////////////////////////////////////////////////////////
vector<vector<Pixel>> process_9(const vector<vector<Pixel>>& image, double scaling_factor)
{
   double r= image.size(); //height 
    int c= image[0].size();//width

    vector<vector<Pixel>> vec10(r, vector<Pixel> (c));

    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            double blue_value = image[row][col].blue;  
            double red_value = image[row][col].red;
            double green_value = image[row][col].green;
            int newred = red_value*scaling_factor;//darkening the pixels
            int newgreen = green_value*scaling_factor;
            int newblue = blue_value*scaling_factor;
                
            
            
            vec10[row][col].green=newgreen;//creating new image
            vec10[row][col].red=newred;
            vec10[row][col].blue=newblue;
            
            

        }
    }
    return vec10;  
}
////////////////////////////////////////////////////////////////
vector<vector<Pixel>> process_10(const vector<vector<Pixel>>& image)
{
    
    double r= image.size(); //height 
    int c= image[0].size();//width

    vector<vector<Pixel>> vec11(r, vector<Pixel> (c));

    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            int blue_value = image[row][col].blue;  
            int red_value = image[row][col].red;
            int green_value = image[row][col].green;
            int max_color = max(blue_value, green_value);
            int a = max_color;
            int max_color2 = max(a, red_value);
            int b = max_color2;//getting the highest value between RGB
            
            
            if ((red_value + green_value + blue_value) >= 550) //setting new RGB based on the sum of the RGB from the original image
                {
                int newred = 255;//new RGB
                int newgreen = 255;
                int newblue = 255;
            
                vec11[row][col].green=newgreen;
                vec11[row][col].red=newred;
                vec11[row][col].blue=newblue;
                }
            else if ((red_value + green_value + blue_value) <= 150)
                {
                int newred = 0;
                int newgreen = 0;
                int newblue = 0;
                vec11[row][col].green=newgreen;
                vec11[row][col].red=newred;
                vec11[row][col].blue=newblue;
                }
            else if (b == red_value)//Changing the color of the highest RGB value(if red)
                {
                int newred = 255;
                int newgreen = 0;
                int newblue = 0;
                vec11[row][col].green=newgreen;
                vec11[row][col].red=newred;
                vec11[row][col].blue=newblue;
                }
            else if (b == green_value)//changing the color if green
                {
                int newred = 0;
                int newgreen = 255;
                int newblue = 0;
                vec11[row][col].green=newgreen;
                vec11[row][col].red=newred;
                vec11[row][col].blue=newblue;
                }
            else   //changing the color if blue
                {
                int newred = 0;
                int newgreen = 0;
                int newblue = 255;
                vec11[row][col].green=newgreen;
                vec11[row][col].red=newred;
                vec11[row][col].blue=newblue;
                }
            

        }
    }
    return vec11;
    
    
    
    
}
///////////////////////////////////////////////////////////////
vector<vector<Pixel>> process_11(const vector<vector<Pixel>>& image)
{
    
    int r= image.size(); //height 
    int c= image[0].size();//width

    vector<vector<Pixel>> vec12(r, vector<Pixel> (c));

    for (int row = 0; row < r; row++)
    {
        for (int col = 0; col < c; col++)
        {
            int blue = image[row][col].blue;  
            int red = image[row][col].red;
            int green = image[row][col].green;

            int newred = (red*0.393+green* 0.769 + blue * 0.189);
            int newgreen = (red * 0.349 +green*0.686+ blue * 0.168);
            int newblue = (red * 0.272 + green * 0.534+blue*0.131);// Formula for sepia tone
            
           if (newred > 255) //condition to reduce high pixel value
           { newred = 255;
            vec12[row][col].red=newred;}
            
            
           else if (newgreen > 255)
           {newgreen = 255;
           vec12[row][col].green=newgreen;
            }
            else if (newblue > 255)
            { newblue = 255;
             vec12[row][col].blue=newblue;}
          
            
            
            vec12[row][col].green=newgreen;
            vec12[row][col].red=newred;
            vec12[row][col].blue=newblue;
            //cout<<newgreen<<" "<<newred<<" "<<newblue<<endl; Used cout to check the pixel value
            
            //newR = (R × 0.393 + G × 0.769 + B × 0.189)
//newG = (R × 0.349 + G × 0.686 + B × 0.168)
//newB = (R × 0.272 + G × 0.534 + B × 0.131)

        }
    }
    return vec12;// results with a problem
}
////////////////////////////////////////////////////////////////
    //Variables used in the functions
string option;
    string bmpname=" ";
    bool work =true;
    string output=" ";
    double number;
    int num;
    int num2;
    bool op =true;

void ClearScreen() //a clear screen command that prints blank lines
    {
    cout << string( 6, '\n' );
    }
void CS()
    {
    cout << string( 2, '\n' );
    }

void menu()  //Void function to use as menu
    {
    ClearScreen();
    cout<<"CSPB 1300 Image Processing Application"<<endl;
    cout<<" "<<endl;
    cout<<"Enter input BMP filename: "<<bmpname<<endl;
    
    cout<<"   "<<endl;
    cout<<"IMAGE PROCESSING MENU"<<endl;
    cout<<"   "<<endl;
     cout<<"0) Change image (current: "<<bmpname<<")"<<endl;
     cout<<"1) Vignette"<<endl;
     cout<<"2) Clarendon"<<endl;
     cout<<"3) Grayscale"<<endl;
     cout<<"4) Rotate 90 degrees"<<endl;
     cout<<"5) Rotate multiple 90 degrees"<<endl;
     cout<<"6) Enlarge"<<endl;
     cout<<"7) High contrast"<<endl;
     cout<<"8) Lighten"<<endl;
     cout<<"9) Darken"<<endl;
     cout<<"10) Black, white, red, green, blue"<<endl;
     cout<<"11) Sepia tone"<<endl;

     cout<<" "<<endl;
    }
void msg()  //Void function for messages
{
  cout<<"PROCESS COMPLETED!"<<endl;
         cout<<"  "<<endl;
         cout<<"BACK TO MENU..."<<endl;
         cout<<"  "<<endl;  
}
////**********MAAAAAIIIIIIINNNN***************//////////////////
    
int main()
{
    
    //All the main invocations
    //vector<vector<Pixel>> image = read_image(bmpname);
    //vector<vector<Pixel>> vec2 = process_1(bmpname);
    //vector<vector<Pixel>> vec3 = process_2(image, 0.3);
   // vector<vector<Pixel>> vec4 = process_3(image);
    //vector<vector<Pixel>> vec5 = process_4(image);
    //vector<vector<Pixel>> vec6 = process_5(image,2);
    //vector<vector<Pixel>> vec7 = process_6(image,2,3);
    //vector<vector<Pixel>> vec8 = process_7(image);
    //vector<vector<Pixel>> vec9 = process_8(image,0.5);
    //vector<vector<Pixel>> vec10 = process_9(image,0.5);
    //vector<vector<Pixel>> vec11 = process_10(image);
    //bool success = write_image("process5.bmp", vec6);
    
    ClearScreen(); ///opening
    cout<<"Enter INPUT bmp filename(nameonly):"<<endl; // DOES NOT NEED BMP IN THE NAME
    
            cin>>bmpname;
            bmpname = bmpname +".bmp";
            ifstream file_in;
            file_in.open(bmpname);
            while (file_in.fail()) //condition in case file has a wrong name
            
            {cout<<"File error...Try again(no .bmp needed):"<<endl;
            cin>>bmpname;
            bmpname = bmpname +".bmp";
             file_in.open(bmpname);
            }
            ClearScreen();
            msg();
    

    
    while (work) ///while loop for the menu
    {   
        vector<vector<Pixel>> image = read_image(bmpname);// out of the loop does not update the image

        menu(); // menu bar
        
        cout<<"Enter menu selection (Q to quit): "<<endl; //enter option inside the loop to update

        cin>>option;
        
        if (option== "0")
        {
         cout<<"Change image (current: "<<bmpname<<")"<<endl;
         cout<<"Enter NEW input BMP filename: "<<endl;
            
            cin>>bmpname;
            bmpname = bmpname +".bmp";
            ifstream file_in;
            file_in.open(bmpname);
            while (file_in.fail()) //condition in case file has a wrong name
            
            {cout<<"File error...Try again(no .bmp needed):"<<endl;
            cin>>bmpname;
            bmpname = bmpname +".bmp";
             file_in.open(bmpname);
            }
            
           
            CS();   //CS(), msg(), ClearScreen() are a format                           pattern for all the number options.
            msg();
            ClearScreen();
            continue;   /// continue in the looping
            
            
        }
        
        else if (option== "1")
        {
            cout<<"Vignette selected!"<<endl;
             cout<<"Enter output BMP filename(nameonly): ";
            cin>>output;
            
            output=output+".bmp";
            
            
         vector<vector<Pixel>> vec2 = process_1(image);
         bool success = write_image(output, vec2);
            
            CS();
            msg();
            ClearScreen();
         
         
         continue;
        }
        
        else if (option== "2")
        {
        cout<<"Claredon selected!"<<endl;

        cout<<"Enter scale factor: "<<endl;
            
            while (!(cin>>number)) //If it's not a number, prints error message
            {cout<<"Error... Please, enter scale factor:";
            cin.clear();
            cin.ignore(123,'\n');}
        
         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";
            
         vector<vector<Pixel>> vec3 = process_2(image,number);
         bool success = write_image(output, vec3);
            
         CS();
         msg();
         ClearScreen();
         continue;
        }
        
        
       else if (option== "3")
        {
         cout<<"Grayscale selected!"<<endl;
         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";
            
         vector<vector<Pixel>> vec4 = process_3(image);
         bool success = write_image(output, vec4);
           
         CS();
         msg();
         ClearScreen();
         continue;
        } 
        
        else if (option== "4")
        {
         cout<<"Rotate 90 degrees selected!"<<endl;

         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";
            
         vector<vector<Pixel>> vec5 = process_4(image);
         bool success = write_image(output, vec5);
            
         CS();
         msg();
         ClearScreen();
         continue;
        }
        
        else if (option== "5")
        {
         cout<<"Rotate multiple 90 degrees selected!"<<endl;
         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";   
            
         cout<<"Enter number of 90 degree rotations:"<<endl;
            while (!(cin>>num)) //If it's not a number error message print
            {cout<<"Error... Please, enter the number of rotations:";
            cin.clear();
            cin.ignore(123,'\n');}
            
         
            
         vector<vector<Pixel>> vec6 = process_5(image, num);
         bool success = write_image(output, vec6);
            
         CS();
         msg();
         ClearScreen();
         continue;
        }
        
        else if (option== "6")
        {
         cout<<"Enlarge selected!"<<endl;
         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";   
            cout<<"Enter x scale:"<<endl;  
            while (!(cin>>num)) //If it's not a number error message print
            {cout<<"Error... Please, enter x scale:";
            cin.clear();
            cin.ignore(123,'\n');}
            
            cout<<"Enter y scale:"<<endl;
            while (!(cin>>num2)) //If it's not a number error message print
            {cout<<"Error... Please, enter y scale:";
            cin.clear();
            cin.ignore(123,'\n');}
            
         
            
         vector<vector<Pixel>> vec7 = process_6(image, num,num2);
         bool success = write_image(output, vec7);
            
         CS();
         msg();
         ClearScreen();
         continue;
        }
        
        else if (option== "7")
        {
         cout<<"High contrast selected!"<<endl;

         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";
            
         vector<vector<Pixel>> vec8 = process_7(image);
         bool success = write_image(output, vec8);
            
         CS();
         msg();
         ClearScreen();
         continue;
        }
        else if (option== "8")
        {
         cout<<"Lighten selected!"<<endl;

         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";
         cout<<"Enter scaling factor: "<<endl;
            while (!(cin>>number)) //If it's not a number error message print
            {cout<<"Error... Please, enter scaling factor:";
            cin.clear();
            cin.ignore(123,'\n');}
            
            
         vector<vector<Pixel>> vec9 = process_8(image,number);
         bool success = write_image(output, vec9);
            
         CS();
         msg();
         ClearScreen();
         continue;
        }
        
        else if (option== "9")
        {
         cout<<"Darken selected!"<<endl;

         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";
         cout<<"Enter scaling factor: "<<endl;
            while (!(cin>>number)) //If it's not a number error message print
            {cout<<"Error... Please, enter scaling factor:";
            cin.clear();
            cin.ignore(123,'\n');}
               
            
         vector<vector<Pixel>> vec10 = process_9(image, number);
         bool success = write_image(output, vec10);
            
         CS();
         msg();
         ClearScreen();
         continue;
        }
        
        else if (option== "10")
        {
         cout<<"Black, white, red, green, blue, selected!"<<endl;

         cout<<"Enter output BMP filename(nameonly): "<<endl;
            cin>>output;
            output=output+".bmp";
            
         vector<vector<Pixel>> vec11 = process_10(image);
         bool success = write_image(output, vec11);
            
         CS();
         msg();
         ClearScreen();
         continue;
        }
        
        else if (option== "11")
        {
            cout<<"Sepia tone selected!"<<endl;
             cout<<"Enter output BMP filename(nameonly): ";
            cin>>output;
            
            output=output+".bmp";
            
            
         vector<vector<Pixel>> vec12 = process_11(image);
         bool success = write_image(output, vec12);
            
            CS();
            msg();
            ClearScreen();
         
         
         continue;
        }
        
        
        else if (option =="Q" | option =="q")
        {
            ClearScreen();
            
            cout<<"Thank you!"<<endl; //end message
            cout<<"Bye!"<<endl;
            
            ClearScreen();
             return work=false; // quit loop
        }
        else
        {   
            ClearScreen();
            cout<<"Wrong input. Please, try again!"<<endl; 
         //if the number for the menu doesn't match the loop number, error message prints
            
            cout<<"  "<<endl;
            ClearScreen();
            continue; // change to continue in a loop
        }

    }

    

    return 0;
}