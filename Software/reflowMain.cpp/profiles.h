#ifndef _PROFILES_H
#define _PROFILES_H

#define NUM_PROFILES 4
#define DEFAULT_PROFILE_NUM 0

const static ReflowProfile defaultProfile = {(char*)"Default",
                                             140, 1.0, 90,
                                             170, 0.4, 90,
                                             200, 1.0, 30,
                                             230, 1.5, 30,
                                            };
ReflowProfile* pDefaultProfile = &defaultProfile;

const static ReflowProfile profile1 = {(char*)"Test",
                                       1, 1.0, 60,
                                       1, 0.63, 60,
                                       1, 1.0, 15,
                                       1, 1.5, 15
                                      };
ReflowProfile* pProfile1 = &profile1;

const static ReflowProfile profile3 = {(char*)"SMD291SNL50T3",
                                       150, 1.4, 90,
                                       175, 0.65, 90,
                                       217, 1.5, 30,
                                       245, 1.0, 30
                                      };
ReflowProfile* pProfile3 = &profile3;

const static ReflowProfile profile4 = {(char*)"SMDLTLFP50T3",
                                       90,  0.8, 90,
                                       130, 0.65, 90,
                                       138, 0.3, 30,
                                       165, 0.9, 30
                                      };
ReflowProfile* pProfile4 = &profile4;


ReflowProfile* allProfiles[NUM_PROFILES] = {pDefaultProfile,
                                            pProfile1,
                                            pProfile3,
                                            pProfile4,
                                           };
#endif
