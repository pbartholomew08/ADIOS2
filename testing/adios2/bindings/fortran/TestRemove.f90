program TestRemove
    use small_test_data
    use mpi
    use adios2
    implicit none

    integer(kind=8), dimension(1) :: shape_dims, start_dims, count_dims
    integer :: inx, irank, isize, ierr, i

    integer(kind=8) :: adios, ioWrite, bpWriter, ioRead, bpReader
    integer(kind=8), dimension(12) :: variables

    ! Launch MPI
    call MPI_Init(ierr)
    call MPI_Comm_rank(MPI_COMM_WORLD, irank, ierr)
    call MPI_Comm_size(MPI_COMM_WORLD, isize, ierr)

    ! Application variables
    inx = 10

    ! Variable dimensions
    shape_dims(1) = isize*inx
    start_dims(1) = irank*inx
    count_dims(1) = inx

    ! Create adios handler passing the communicator, debug mode and error flag
    call adios2_init(adios, MPI_COMM_WORLD, adios2_debug_mode_on, ierr)

    !!!!!!!!!!!!!!!!!!!!!!!!! WRITER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ! Declare an IO process configuration inside adios
    call adios2_declare_io(ioWrite, adios, "ioWrite", ierr)

    ! Defines a variable to be written in bp format
    call adios2_define_variable(variables(1), ioWrite, "var_I8", 1, &
                                shape_dims, start_dims, count_dims, &
                                adios2_constant_dims, data_I8, ierr)

    call adios2_define_variable(variables(2), ioWrite, "var_I16", 1, &
                                shape_dims, start_dims, count_dims, &
                                adios2_constant_dims, data_I16, ierr)

    call adios2_define_variable(variables(3), ioWrite, "var_I32", 1, &
                                shape_dims, start_dims, count_dims, &
                                adios2_constant_dims, data_I32, ierr)

    call adios2_define_variable(variables(4), ioWrite, "var_I64", 1, &
                                shape_dims, start_dims, count_dims, &
                                adios2_constant_dims, data_I64, ierr)

    call adios2_define_variable(variables(5), ioWrite, "var_R32", 1, &
                                shape_dims, start_dims, count_dims, &
                                adios2_constant_dims, data_R32, ierr)

    call adios2_define_variable(variables(6), ioWrite, "var_R64", 1, &
                                shape_dims, start_dims, count_dims, &
                                adios2_constant_dims, data_R64, ierr)

    ! Global variables
    call adios2_define_variable(variables(7), ioWrite, "gvar_I8", data_I8(1), &
                                ierr)

    call adios2_define_variable(variables(8), ioWrite, "gvar_I16", &
                                data_I16(1), ierr)

    call adios2_define_variable(variables(9), ioWrite, "gvar_I32", &
                                data_I32(1), ierr)

    call adios2_define_variable(variables(10), ioWrite, "gvar_I64", &
                                data_I64(1), ierr)

    call adios2_define_variable(variables(11), ioWrite, "gvar_R32", &
                                data_R32(1), ierr)

    call adios2_define_variable(variables(12), ioWrite, "gvar_R64", &
                                data_R64(1), ierr)

    ! remove piece
    call adios2_remove_variable(ioWrite, "gvar_R64", ierr)
    if (ierr /= 1) stop 'gvar_R64 not removed'

    call adios2_inquire_variable(variables(12), ioWrite, "gvar_R64", ierr)
    if (ierr == adios2_found) stop 'gvar_R64 found with inquire, not removed'

    ! remove all
    call adios2_remove_all_variables(ioWrite, ierr)

    call adios2_inquire_variable(variables(1), ioWrite, "var_I8", ierr)
    if (ierr == adios2_found) stop 'var_I8 found'

    call adios2_inquire_variable(variables(2), ioWrite, "var_I16", ierr)
    if (ierr == adios2_found) stop 'var_I16 found'

    call adios2_inquire_variable(variables(3), ioWrite, "var_I32", ierr)
    if (ierr == adios2_found) stop 'var_I32 found'

    call adios2_inquire_variable(variables(4), ioWrite, "var_I64", ierr)
    if (ierr == adios2_found) stop 'var_I64 found'

    call adios2_inquire_variable(variables(5), ioWrite, "var_R32", ierr)
    if (ierr == adios2_found) stop 'var_R32 found'

    call adios2_inquire_variable(variables(6), ioWrite, "var_R64", ierr)
    if (ierr == adios2_found) stop 'var_R64 found'

    call adios2_inquire_variable(variables(7), ioWrite, "gvar_I8", ierr)
    if (ierr == adios2_found) stop 'gvar_I8 found'

    call adios2_inquire_variable(variables(8), ioWrite, "gvar_I16", ierr)
    if (ierr == adios2_found) stop 'gvar_I16 found'

    call adios2_inquire_variable(variables(9), ioWrite, "gvar_I32", ierr)
    if (ierr == adios2_found) stop 'gvar_I32 found'

    call adios2_inquire_variable(variables(10), ioWrite, "gvar_I64", ierr)
    if (ierr == adios2_found) stop 'gvar_I64 found'

    call adios2_inquire_variable(variables(11), ioWrite, "gvar_R32", ierr)
    if (ierr == adios2_found) stop 'gvar_R32 found'

    call adios2_finalize(adios, ierr)

    call MPI_Finalize(ierr)

end program TestRemove