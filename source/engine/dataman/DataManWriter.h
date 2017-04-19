/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * DataMan.h
 *
 *  Created on: Jan 10, 2017
 *      Author: wfg
 */

#ifndef DATAMANWRITER_H_
#define DATAMANWRITER_H_

#include <iostream> //std::cout must be removed, only used for hello example
#include <unistd.h> //sleep must be removed

#include "ADIOSConfig.h"
#include "core/Engine.h"
#include "utilities/format/bp1/BP1Writer.h"
#include "utilities/realtime/dataman/DataMan.h"
#include "capsule/heap/STLVector.h"

namespace adios
{

class DataManWriter : public Engine
{

public:
    using json = nlohmann::json;
    /**
     * Constructor for dataman engine Writer for WAN communications
     * @param adios
     * @param name unique name given to the engine
     * @param accessMode
     * @param mpiComm
     * @param method
     * @param debugMode
     * @param nthreads
     */
    DataManWriter(ADIOS &adios, const std::string name,
                  const std::string accessMode, MPI_Comm mpiComm,
                  const Method &method);

    virtual ~DataManWriter() = default;

    void SetCallBack(std::function<void(const void *, std::string, std::string,
                                        std::string, Dims)>
                         callback);

    void Write(Variable<char> &variable, const char *values);
    void Write(Variable<unsigned char> &variable, const unsigned char *values);
    void Write(Variable<short> &variable, const short *values);
    void Write(Variable<unsigned short> &variable,
               const unsigned short *values);
    void Write(Variable<int> &variable, const int *values);
    void Write(Variable<unsigned int> &variable, const unsigned int *values);
    void Write(Variable<long int> &variable, const long int *values);
    void Write(Variable<unsigned long int> &variable,
               const unsigned long int *values);
    void Write(Variable<long long int> &variable, const long long int *values);
    void Write(Variable<unsigned long long int> &variable,
               const unsigned long long int *values);
    void Write(Variable<float> &variable, const float *values);
    void Write(Variable<double> &variable, const double *values);
    void Write(Variable<long double> &variable, const long double *values);
    void Write(Variable<std::complex<float>> &variable,
               const std::complex<float> *values);
    void Write(Variable<std::complex<double>> &variable,
               const std::complex<double> *values);
    void Write(Variable<std::complex<long double>> &variable,
               const std::complex<long double> *values);

    void Write(const std::string &variableName, const char *values);
    void Write(const std::string &variableName, const unsigned char *values);
    void Write(const std::string &variableName, const short *values);
    void Write(const std::string &variableName, const unsigned short *values);
    void Write(const std::string &variableName, const int *values);
    void Write(const std::string &variableName, const unsigned int *values);
    void Write(const std::string &variableName, const long int *values);
    void Write(const std::string &variableName,
               const unsigned long int *values);
    void Write(const std::string &variableName, const long long int *values);
    void Write(const std::string &variableName,
               const unsigned long long int *values);
    void Write(const std::string &variableName, const float *values);
    void Write(const std::string &variableName, const double *values);
    void Write(const std::string &variableName, const long double *values);
    void Write(const std::string &variableName,
               const std::complex<float> *values);
    void Write(const std::string &variableName,
               const std::complex<double> *values);
    void Write(const std::string &variableName,
               const std::complex<long double> *values);

    void Close(const int transportIndex = -1);

private:
    capsule::STLVector
        m_Buffer; ///< heap capsule, contains data and metadata buffers
    format::BP1Writer
        m_BP1Writer; ///< format object will provide the required BP
                     /// functionality to be applied on m_Buffer and
    /// m_Transports

    bool m_DoRealTime = false;
    bool m_DoMonitor = false;
    realtime::DataMan m_Man;
    std::function<void(const void *, std::string, std::string, std::string,
                       Dims)>
        m_CallBack; ///< call back function

    void Init(); ///< calls InitCapsules and InitTransports based on Method,
                 /// called from constructor
    void InitTransports(); ///< from Transports

    /**
     * From transport Mdtm in m_Method
     * @param parameter must be an accepted parameter
     * @param mdtmParameters
     * @return value either returns user-defined from "parameter=value" or a
     * default
     */
    std::string
    GetMdtmParameter(const std::string parameter,
                     const std::map<std::string, std::string> &mdtmParameters);

    template <class T>
    void WriteVariableCommon(Variable<T> &variable, const T *values)
    {
        // here comes your magic at Writing now variable.m_UserValues has the
        // data
        // passed by the user
        // set variable
        variable.m_AppValues = values;
        m_WrittenVariables.insert(variable.m_Name);

        // This part will go away, this is just to monitor variables per rank

        json jmsg;
        jmsg["doid"] = m_Name;
        jmsg["var"] = variable.m_Name;
        jmsg["dtype"] = GetType<T>();
        jmsg["putshape"] = variable.m_LocalDimensions;
        if (variable.m_GlobalDimensions.size() == 0)
            variable.m_GlobalDimensions = variable.m_LocalDimensions;
        jmsg["varshape"] = variable.m_GlobalDimensions;
        if (variable.m_Offsets.size() == 0)
            variable.m_Offsets.assign(variable.m_LocalDimensions.size(), 0);
        jmsg["offset"] = variable.m_Offsets;
        jmsg["timestep"] = 0;
        m_Man.put(values, jmsg);

        if (m_DoMonitor)
        {
            MPI_Barrier(m_MPIComm);
            std::cout << "I am hooked to the DataMan library\n";
            std::cout << "putshape " << variable.m_LocalDimensions.size()
                      << std::endl;
            std::cout << "varshape " << variable.m_GlobalDimensions.size()
                      << std::endl;
            std::cout << "offset " << variable.m_Offsets.size() << std::endl;
            for (int i = 0; i < m_SizeMPI; ++i)
            {
                if (i == m_RankMPI)
                {
                    std::cout << "Rank: " << m_RankMPI << "\n";
                    variable.Monitor(std::cout);
                    std::cout << std::endl;
                }
                else
                {
                    sleep(1);
                }
            }
            MPI_Barrier(m_MPIComm);
        }
    }
};

} // end namespace adios

#endif /* DATAMANWRITER_H_ */
