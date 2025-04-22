module c_constants_mod

  !Constants from FMS
  !See FMS/constants/gfdl_constants.fh for further documentation
  
  use constants_mod, only: PI, RAD_TO_DEG, DEG_TO_RAD, RADIAN,                        &
       RADIUS, OMEGA, GRAV, SECONDS_PER_DAY, SECONDS_PER_HOUR, SECONDS_PER_MINUTE,    &
       RDGAS, RVGAS, HLV, HLF, HLS, KAPPA, CP_AIR, CP_VAPOR, CP_OCEAN, DENS_H2O,      &
       RHOAIR, RHO0, RHO0R, RHO_CP, O2MIXRAT, WTMAIR, WTMH2O, WTMOZONE, WTMC, WTMCO2, &
       WTMCH4, WTMO2, WTMCFC11, WTMCFC12, WTMN, DIFFAC, ES0, PSTD, PSTD_MKS, KELVIN, TFREEZE, &
       C2DBARS, STEFAN, AVOGNO, VONKARM, ALOGMIN, EPSLN, RADCON, RADCON_MKS
  
  use iso_c_binding

  implicit none

  public
  
  real(c_double), bind(C, name="PI")                 :: PI_C = PI

  real(c_double), bind(C, name="RAD_TO_DEG")         :: RAD_TO_DEG_C = RAD_TO_DEG
  real(c_double), bind(C, name="DEG_TO_RAD")         :: DEG_TO_RAD_C = DEG_TO_RAD  
  real(c_double), bind(C, name="RADIAN")             :: RADIAN_C = RADIAN

  real(c_double), bind(C, name="RADIUS")             :: RADIUS_C = RADIUS
  real(c_double), bind(C, name="OMEGA")              :: OMEGA_C = OMEGA
  real(c_double), bind(C, name="GRAV")               :: GRAV_C = GRAV
  real(c_double), bind(C, name="SECONDS_PER_DAY")    :: SECONDS_PER_DAY_C = SECONDS_PER_DAY
  real(c_double), bind(C, name="SECONDS_PER_HOUR")   :: SECONDS_PER_HOUR_C = SECONDS_PER_HOUR
  real(c_double), bind(C, name="SECONDS_PER_MINUTE") :: SECONDS_PER_MINUTE_C = SECONDS_PER_MINUTE
  real(c_double), bind(C, name="RDGAS")              :: RDGAS_C = RDGAS
  real(c_double), bind(C, name="RVGAS")              :: RVGAS_C = RVGAS
  real(c_double), bind(C, name="HLV")                :: HLV_C = HLV
  real(c_double), bind(C, name="HLS")                :: HLS_C = HLS
  real(c_double), bind(C, name="KAPPA")              :: KAPPA_C = KAPPA
  real(c_double), bind(C, name="CP_AIR")             :: CP_AIR_C = CP_AIR
  real(c_double), bind(C, name="CP_VAPOR")           :: CP_VAPOR_C = CP_VAPOR
  real(c_double), bind(C, name="CP_OCEAN")           :: CP_OCEAN_C = CP_OCEAN
  real(c_double), bind(C, name="DENS_H20")           :: DENS_H2O_C = DENS_H2O
  real(c_double), bind(C, name="RHOAIR")             :: RHOAIR_C = RHOAIR
  real(c_double), bind(C, name="RHO0")               :: RHO0_C = RHO0
  real(c_double), bind(C, name="RHO0R")              :: RHO0R_C = RHO0R
  real(c_double), bind(C, name="RHO_CP")             :: RHO_CP_C = RHO_CP
  real(c_double), bind(C, name="O2MIXRAT")           :: O2MIXRAT_C = O2MIXRAT
  real(c_double), bind(C, name="WTMAIR")             :: WTMAIR_C = WTMAIR
  real(c_double), bind(C, name="WTMH2O")             :: WTMH2O_C = WTMH2O
  real(c_double), bind(C, name="WTMOZONE")           :: WTMOZONE_C = WTMOZONE
  real(c_double), bind(C, name="WTMC")               :: WTMC_C = WTMC
  real(c_double), bind(C, name="WTMCO2")             :: WTMCO2_C = WTMCO2
  real(c_double), bind(C, name="WTMCH4")             :: WTMCH4_C = WTMCH4
  real(c_double), bind(C, name="WTMO2")              :: WTMO2_C = WTMO2
  real(c_double), bind(C, name="WTMCFC11")           :: WTMCFC11_C = WTMCFC11
  real(c_double), bind(C, name="WTMCFC12")           :: WTMCFC12_C = WTMCFC12
  real(c_double), bind(C, name="WTMN")               :: WTMN_C = WTMN
  real(c_double), bind(C, name="DIFFAC")             :: DIFFAC_C = DIFFAC
  real(c_double), bind(C, name="ES0")                :: ES0_C = ES0
  real(c_double), bind(C, name="PSTD")               :: PSTD_C = PSTD
  real(c_double), bind(C, name="PSTD_MKS")           :: PSTD_MKS_C = PSTD_MKS
  real(c_double), bind(C, name="KELVIN")             :: KELVIN_C = KELVIN
  real(c_double), bind(C, name="TFREEZE")            :: TFREEZE_C = TFREEZE
  real(c_double), bind(C, name="C2DBARS")            :: C2DBARS_C = C2DBARS
  real(c_double), bind(C, name="STEFAN")             :: STEFAN_C = STEFAN
  real(c_double), bind(C, name="AVOGNO")             :: AVOGNO_C = AVOGNO
  real(c_double), bind(C, name="VONKARM")            :: VONKARM_C = VONKARM
  real(c_double), bind(C, name="ALOGMIN")            :: ALOGMIN_C = ALOGMIN
  real(c_double), bind(C, name="EPSLN")              :: EPSLN_C = EPSLN
  real(c_double), bind(C, name="RADCON")             :: RADCON_C = RADCON
  real(c_double), bind(C, name="RADCON_MKS")         :: RADCON_MKS_C = RADCON_MKS
  
  
end module c_constants_mod
