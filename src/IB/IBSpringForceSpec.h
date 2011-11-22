// Filename: IBSpringForceSpec.h
// Created on 14 Jul 2004 by Boyce Griffith
//
// Copyright (c) 2002-2010, Boyce Griffith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of New York University nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef included_IBSpringForceSpec
#define included_IBSpringForceSpec

/////////////////////////////// INCLUDES /////////////////////////////////////

#ifndef included_IBAMR_prefix_config
#include <IBAMR_prefix_config.h>
#define included_IBAMR_prefix_config
#endif

// IBTK INCLUDES
#include <ibtk/Streamable.h>

// SAMRAI INCLUDES
#include <tbox/AbstractStream.h>

// C++ STDLIB INCLUDES
#include <vector>

/////////////////////////////// FORWARD DECLARATION //////////////////////////

namespace IBAMR
{
class IBSpringForceSpecFactory;
}

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{
/*!
 * \brief Class IBSpringForceSpec encapsulates the data necessary to compute the
 * forces generated by a network of linear or nonlinear springs (i.e.,
 * structures that resist extension and/or compression) at a single node of the
 * Lagrangian mesh.
 *
 * Springs are connections between two particular nodes of the Lagrangian mesh.
 * Forces generated by springs have the feature that the force applied to one
 * end of the spring is the negation of the force applied to the other end of
 * the spring.  Consequently, to determine the total nodal forces, we only need
 * to \em compute the force at one end of the spring (e.g., at the master node).
 * Then, the negation of that force can be applied to the opposite end of the
 * spring (e.g., at the slave node).
 *
 * IBSpringForceSpec objects are stored as IBTK::Streamable data associated with
 * only the master nodes in the mesh.
 *
 * \note Different spring force functions may be specified for each link in the
 * mesh.  This data is specified as \a force_fcn_idxs in the class constructor.
 * By default, function default_linear_spring_force() is associated with \a
 * force_fcn_idx 0.  Users may override this default value with any function
 * that implements the interface required by
 * IBSpringForceGen::registerSpringForceFunction().
 */
class IBSpringForceSpec
    : public IBTK::Streamable
{
public:
    friend class IBSpringForceSpecFactory;

    /*!
     * \brief Register this class and its factory class with the singleton
     * IBTK::StreamableManager object.  This method must be called before any
     * IBSpringForceSpec objects are created.
     *
     * \note This method is collective on all MPI processes.  This is done to
     * ensure that all processes employ the same class ID for the
     * IBSpringForceSpec class.
     */
    static void
    registerWithStreamableManager();

    /*!
     * \brief Returns a boolean indicating whether the class has been registered
     * with the singleton IBTK::StreamableManager object.
     */
    static bool
    getIsRegisteredWithStreamableManager();

    /*!
     * The unique class ID for this object type assigned by the
     * IBTK::StreamableManager.
     */
    static int STREAMABLE_CLASS_ID;

    /*!
     * \brief Default constructor.
     */
    IBSpringForceSpec(
        unsigned int num_springs=0);

    /*!
     * \brief Alternative constructor.
     */
    IBSpringForceSpec(
        int master_idx,
        const std::vector<int>& slave_idxs,
        const std::vector<int>& force_fcn_idxs,
        const std::vector<double>& stiffnesses,
        const std::vector<double>& rest_lengths
#if ENABLE_SUBDOMAIN_INDICES
        ,const std::vector<int>& subdomain_idxs
#endif
                      );

    /*!
     * \brief Destructor.
     */
    ~IBSpringForceSpec();

    /*!
     * \return The number of springs attached to the master node.
     */
    unsigned int
    getNumberOfSprings() const;

    /*!
     * \return A const reference to the master node index.
     */
    const int&
    getMasterNodeIndex() const;

    /*!
     * \return A non-const reference to the master node index.
     */
    int&
    getMasterNodeIndex();

    /*!
     * \return A const reference to the slave node indices for the springs
     * attached to the master node.
     */
    const std::vector<int>&
    getSlaveNodeIndices() const;

    /*!
     * \return A non-const reference to the slave node indices for the springs
     * attached to the master node.
     */
    std::vector<int>&
    getSlaveNodeIndices();

    /*!
     * \return A const reference to the force function index of the springs
     * attached to the master node.
     */
    const std::vector<int>&
    getForceFunctionIndices() const;

    /*!
     * \return A non-const reference to the force function index of the springs
     * attached to the master node.
     */
    std::vector<int>&
    getForceFunctionIndices();

    /*!
     * \return A const reference to the stiffnesses of the springs attached to
     * the master node.
     */
    const std::vector<double>&
    getStiffnesses() const;

    /*!
     * \return A non-const reference to the stiffnesses of the springs attached
     * to the master node.
     */
    std::vector<double>&
    getStiffnesses();

    /*!
     * \return A const reference to the resting length of the springs attached
     * to the master node.
     */
    const std::vector<double>&
    getRestingLengths() const;

    /*!
     * \return A non-const reference to the resting length of the springs
     * attached to the master node.
     */
    std::vector<double>&
    getRestingLengths();

#if ENABLE_SUBDOMAIN_INDICES
    /*!
     * \return A const reference to the subdomain indices associated with this
     * force spec object.
     */
    const std::vector<int>&
    getSubdomainIndices() const;

    /*!
     * \return A non-const reference to the subdomain indices associated with
     * this force spec object.
     */
    std::vector<int>&
    getSubdomainIndices();
#endif

    /*!
     * \brief Return the unique identifier used to specify the
     * IBTK::StreamableFactory object used by the IBTK::StreamableManager to
     * extract Streamable objects from data streams.
     */
    int
    getStreamableClassID() const;

    /*!
     * \brief Return an upper bound on the amount of space required to pack the
     * object to a buffer.
     */
    size_t
    getDataStreamSize() const;

    /*!
     * \brief Pack data into the output stream.
     */
    void
    packStream(
        SAMRAI::tbox::AbstractStream& stream);

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    IBSpringForceSpec(
        const IBSpringForceSpec& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    IBSpringForceSpec&
    operator=(
        const IBSpringForceSpec& that);

    /*!
     * Data required to define the spring forces.
     */
    int d_master_idx;
    std::vector<int> d_slave_idxs, d_force_fcn_idxs;
    std::vector<double> d_stiffnesses, d_rest_lengths;

#if ENABLE_SUBDOMAIN_INDICES
    /*!
     * The subdomain indices of the force spec object.
     */
    std::vector<int> d_subdomain_idxs;
#endif
};
}// namespace IBAMR

/////////////////////////////// INLINE ///////////////////////////////////////

#include <ibamr/IBSpringForceSpec.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_IBSpringForceSpec
