
#pragma once
#ifndef _CMD5Ex_H
#define _CMD5Ex_H

#ifndef DLLGENERALMODULAR_API
#ifndef __NO_USE_GENERALMODULAR_DLL
#ifdef DLLGENERALMODULAR_EXPORTS
#define DLLGENERALMODULAR_API __declspec(dllexport)
#else
#define DLLGENERALMODULAR_API __declspec(dllimport)
#endif /*DLLGENERALMODULAR_EXPORTS*/
#else
#define DLLGENERALMODULAR_API 
#endif /*__NO_USE_GENERALMODULAR_DLL*/
#endif/*DLLGENERALMODULAR_API*/

//#pragma warning(disable:4786)

#include <string>

using namespace std;

/*!
 * Manage CMD5Ex.
 */
class CMD5Ex
{
private:
    #define uint8  unsigned char
    #define uint32 unsigned long int

    struct CMD5Ex_context
    {
        uint32 total[2];
        uint32 state[4];
        uint8 buffer[64];
    };

    void CMD5Ex_starts( struct CMD5Ex_context *ctx );
    void CMD5Ex_process( struct CMD5Ex_context *ctx, uint8 data[64] );
    void CMD5Ex_update( struct CMD5Ex_context *ctx, uint8 *input, uint32 length );
    void CMD5Ex_finish( struct CMD5Ex_context *ctx, uint8 digest[16] );

public:
    //! construct a CMD5Ex from any buffer
    void GenerateCMD5Ex(unsigned char* buffer,int bufferlen);

    //! construct a CMD5Ex
    CMD5Ex();

    //! construct a CMD5Exsrc from char *
    CMD5Ex(const char * CMD5Exsrc);

    //! construct a CMD5Ex from a 16 bytes CMD5Ex
    CMD5Ex(unsigned long* CMD5Exsrc);

    //! add a other CMD5Ex
    CMD5Ex operator +(CMD5Ex adder);

    //! just if equal
    bool operator ==(CMD5Ex cmper);

    //! give the value from equer
    // void operator =(CMD5Ex equer);

    //! to a string
    string ToString();

    unsigned long m_data[4];
};
#endif /* CMD5Ex.h */
