//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Second_att_control_codeblock_fly.h
//
// Code generated for Simulink model 'Second_att_control_codeblock_fly'.
//
// Model version                  : 1.29
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Mon Aug 10 17:49:21 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_Second_att_control_codeblock_fly_h_
#define RTW_HEADER_Second_att_control_codeblock_fly_h_
#include <cmath>
#include "rtwtypes.h"
#include "Second_att_control_codeblock_fly_types.h"

// Macros for accessing real-time model data structure

// Class declaration for model Second_att_control_codeblock_fly
class px4_AlgorithmModelClass {
  // public data and function members
 public:
  // Block signals (default storage)
  typedef struct {
    real32_T DataTypeConversion;       // '<S3>/Data Type Conversion'
    real32_T Fcn2;                     // '<S3>/Fcn2'
    real32_T Saturation9;              // '<S3>/Saturation9'
    real32_T DataTypeConversion1;      // '<S3>/Data Type Conversion1'
    real32_T Fcn5;                     // '<S3>/Fcn5'
    real32_T Saturation8;              // '<S3>/Saturation8'
    real32_T Gain2[2];                 // '<S3>/Gain2'
    real32_T Gain[2];                  // '<S3>/Gain'
    real32_T pitchrollerror;           // '<S5>/Sum'
    real32_T P_pr;                     // '<S5>/P_pr'
    real32_T DiscreteTimeIntegrator;   // '<S5>/Discrete-Time Integrator'
    real32_T I_pr;                     // '<S5>/I_pr'
    real32_T P_pr_m;                   // '<S5>/Sum16'
    real32_T pitchrollerror_b;         // '<S6>/Sum'
    real32_T P_pr_b;                   // '<S6>/P_pr'
    real32_T DiscreteTimeIntegrator_h; // '<S6>/Discrete-Time Integrator'
    real32_T I_pr_d;                   // '<S6>/I_pr'
    real32_T TSamp;                    // '<S8>/TSamp'
    real32_T Uk1;                      // '<S8>/UD'
    real32_T Diff;                     // '<S8>/Diff'
    real32_T D_pr;                     // '<S6>/D_pr'
    real32_T P_pr_d;                   // '<S6>/Sum16'
    real32_T Saturation;               // '<S6>/Saturation'
    real32_T Gain_p;                   // '<S6>/Gain'
    real32_T pitchrollerror_m;         // '<S5>/Sum2'
    real32_T P_pr1;                    // '<S5>/P_pr1'
    real32_T DiscreteTimeIntegrator1;  // '<S5>/Discrete-Time Integrator1'
    real32_T I_pr1;                    // '<S5>/I_pr1'
    real32_T P_pr_o;                   // '<S5>/Sum1'
    real32_T pitchrollerror_i;         // '<S6>/Sum1'
    real32_T P_pr1_p;                  // '<S6>/P_pr1'
    real32_T DiscreteTimeIntegrator1_p;// '<S6>/Discrete-Time Integrator1'
    real32_T I_pr1_j;                  // '<S6>/I_pr1'
    real32_T TSamp_h;                  // '<S9>/TSamp'
    real32_T Uk1_i;                    // '<S9>/UD'
    real32_T Diff_l;                   // '<S9>/Diff'
    real32_T D_pr1;                    // '<S6>/D_pr1'
    real32_T P_pr_j;                   // '<S6>/Sum2'
    real32_T Saturation1;              // '<S6>/Saturation1'
    real32_T Gain1;                    // '<S6>/Gain1'
    real32_T DataTypeConversion2;      // '<S3>/Data Type Conversion2'
    real32_T Fcn7;                     // '<S3>/Fcn7'
    real32_T Saturation7;              // '<S3>/Saturation7'
    real32_T Sum;                      // '<S7>/Sum'
    real32_T Gain_f;                   // '<S7>/Gain'
    real32_T ProportionalGain;         // '<S47>/Proportional Gain'
    real32_T Integrator;               // '<S42>/Integrator'
    real32_T Sum_f;                    // '<S51>/Sum'
    real32_T DataTypeConversion3;      // '<S3>/Data Type Conversion3'
    real32_T Fcn6;                     // '<S3>/Fcn6'
    real32_T Saturation10;             // '<S3>/Saturation10'
    real32_T Delay;                    // '<S6>/Delay'
    real32_T antiWU_Gain;              // '<S6>/antiWU_Gain'
    real32_T Add;                      // '<S6>/Add'
    real32_T Delay1;                   // '<S6>/Delay1'
    real32_T antiWU_Gain1;             // '<S6>/antiWU_Gain1'
    real32_T Add1;                     // '<S6>/Add1'
    real32_T IntegralGain;             // '<S39>/Integral Gain'
    uint16_T M1;                       // '<S1>/pwm_out1'
    uint16_T M2;                       // '<S1>/pwm_out1'
    uint16_T M3;                       // '<S1>/pwm_out1'
    uint16_T M4;                       // '<S1>/pwm_out1'
    uint16_T M5;                       // '<S1>/pwm_out1'
    uint16_T M6;                       // '<S1>/pwm_out1'
    uint16_T Saturation_o[6];          // '<S1>/Saturation'
  } B_Second_att_control_codebloc_T;

  // Block states (default storage) for system '<Root>'
  typedef struct {
    real32_T DiscreteTimeIntegrator_DSTATE;// '<S5>/Discrete-Time Integrator'
    real32_T DiscreteTimeIntegrator_DSTATE_k;// '<S6>/Discrete-Time Integrator'
    real32_T UD_DSTATE;                // '<S8>/UD'
    real32_T DiscreteTimeIntegrator1_DSTATE;// '<S5>/Discrete-Time Integrator1'
    real32_T DiscreteTimeIntegrator1_DSTAT_d;// '<S6>/Discrete-Time Integrator1' 
    real32_T UD_DSTATE_e;              // '<S9>/UD'
    real32_T Integrator_DSTATE;        // '<S42>/Integrator'
    real32_T Delay_DSTATE;             // '<S6>/Delay'
    real32_T Delay1_DSTATE;            // '<S6>/Delay1'
  } DW_Second_att_control_codeblo_T;

  // External inputs (root inport signals with default storage)
  typedef struct {
    real32_T Roll;                     // '<Root>/Roll'
    real32_T Pitch;                    // '<Root>/Pitch'
    real32_T p;                        // '<Root>/p'
    real32_T q;                        // '<Root>/q'
    real32_T r;                        // '<Root>/r'
    uint16_T set_thrust;               // '<Root>/set_thrust'
    uint16_T set_roll;                 // '<Root>/set_roll'
    uint16_T set_pitch;                // '<Root>/set_pitch'
    uint16_T set_yaw;                  // '<Root>/set_yaw'
    uint16_T Arm_cmd;                  // '<Root>/Arm_cmd'
  } ExtU_Second_att_control_codeb_T;

  // External outputs (root outports fed by signals with default storage)
  typedef struct {
    uint16_T PWM_OUT[6];               // '<Root>/PWM_OUT'
  } ExtY_Second_att_control_codeb_T;

  // Parameters (default storage)
  struct P_Second_att_control_codebloc_T {
    real32_T DiscretePIDController_I; // Mask Parameter: DiscretePIDController_I
                                         //  Referenced by: '<S39>/Integral Gain'

    real32_T DiscreteDerivative_ICPrevScaled;
                              // Mask Parameter: DiscreteDerivative_ICPrevScaled
                                 //  Referenced by: '<S8>/UD'

    real32_T DiscreteDerivative1_ICPrevScale;
                              // Mask Parameter: DiscreteDerivative1_ICPrevScale
                                 //  Referenced by: '<S9>/UD'

    real32_T DiscretePIDController_InitialCo;
                              // Mask Parameter: DiscretePIDController_InitialCo
                                 //  Referenced by: '<S42>/Integrator'

    real32_T DiscretePIDController_P; // Mask Parameter: DiscretePIDController_P
                                         //  Referenced by: '<S47>/Proportional Gain'

    real32_T Saturation9_UpperSat;   // Computed Parameter: Saturation9_UpperSat
                                        //  Referenced by: '<S3>/Saturation9'

    real32_T Saturation9_LowerSat;   // Computed Parameter: Saturation9_LowerSat
                                        //  Referenced by: '<S3>/Saturation9'

    real32_T Saturation8_UpperSat;   // Computed Parameter: Saturation8_UpperSat
                                        //  Referenced by: '<S3>/Saturation8'

    real32_T Saturation8_LowerSat;   // Computed Parameter: Saturation8_LowerSat
                                        //  Referenced by: '<S3>/Saturation8'

    real32_T Gain2_Gain[2];            // Computed Parameter: Gain2_Gain
                                          //  Referenced by: '<S3>/Gain2'

    real32_T Gain_Gain;                // Expression: Max_angle_coef
                                          //  Referenced by: '<S3>/Gain'

    real32_T P_pr_Gain;                // Expression: Angle_rate_roll_PI_kP
                                          //  Referenced by: '<S5>/P_pr'

    real32_T DiscreteTimeIntegrator_gainval;
                           // Computed Parameter: DiscreteTimeIntegrator_gainval
                              //  Referenced by: '<S5>/Discrete-Time Integrator'

    real32_T DiscreteTimeIntegrator_IC;
                                // Computed Parameter: DiscreteTimeIntegrator_IC
                                   //  Referenced by: '<S5>/Discrete-Time Integrator'

    real32_T I_pr_Gain;                // Expression: Angle_rate_roll_PI_kI
                                          //  Referenced by: '<S5>/I_pr'

    real32_T P_pr_Gain_a;              // Expression: kP_Roll_rate_PID
                                          //  Referenced by: '<S6>/P_pr'

    real32_T DiscreteTimeIntegrator_gainva_l;
                          // Computed Parameter: DiscreteTimeIntegrator_gainva_l
                             //  Referenced by: '<S6>/Discrete-Time Integrator'

    real32_T DiscreteTimeIntegrator_IC_k;
                              // Computed Parameter: DiscreteTimeIntegrator_IC_k
                                 //  Referenced by: '<S6>/Discrete-Time Integrator'

    real32_T I_pr_Gain_d;              // Expression: kI_Roll_rate_PID
                                          //  Referenced by: '<S6>/I_pr'

    real32_T TSamp_WtEt;               // Computed Parameter: TSamp_WtEt
                                          //  Referenced by: '<S8>/TSamp'

    real32_T D_pr_Gain;                // Expression: kD_Roll_rate_PID
                                          //  Referenced by: '<S6>/D_pr'

    real32_T Saturation_UpperSat;     // Computed Parameter: Saturation_UpperSat
                                         //  Referenced by: '<S6>/Saturation'

    real32_T Saturation_LowerSat;     // Computed Parameter: Saturation_LowerSat
                                         //  Referenced by: '<S6>/Saturation'

    real32_T Gain_Gain_p;              // Computed Parameter: Gain_Gain_p
                                          //  Referenced by: '<S6>/Gain'

    real32_T P_pr1_Gain;               // Expression: Angle_rate_pitch_kP
                                          //  Referenced by: '<S5>/P_pr1'

    real32_T DiscreteTimeIntegrator1_gainval;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainval
                             //  Referenced by: '<S5>/Discrete-Time Integrator1'

    real32_T DiscreteTimeIntegrator1_IC;
                               // Computed Parameter: DiscreteTimeIntegrator1_IC
                                  //  Referenced by: '<S5>/Discrete-Time Integrator1'

    real32_T I_pr1_Gain;               // Expression: Angle_rate_pitch_kI
                                          //  Referenced by: '<S5>/I_pr1'

    real32_T P_pr1_Gain_n;             // Expression: kP_Pitch_rate_PID
                                          //  Referenced by: '<S6>/P_pr1'

    real32_T DiscreteTimeIntegrator1_gainv_k;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainv_k
                             //  Referenced by: '<S6>/Discrete-Time Integrator1'

    real32_T DiscreteTimeIntegrator1_IC_j;
                             // Computed Parameter: DiscreteTimeIntegrator1_IC_j
                                //  Referenced by: '<S6>/Discrete-Time Integrator1'

    real32_T I_pr1_Gain_g;             // Expression: kI_Pitch_rate_PID
                                          //  Referenced by: '<S6>/I_pr1'

    real32_T TSamp_WtEt_l;             // Computed Parameter: TSamp_WtEt_l
                                          //  Referenced by: '<S9>/TSamp'

    real32_T D_pr1_Gain;               // Expression: kD_Pitch_rate_PID
                                          //  Referenced by: '<S6>/D_pr1'

    real32_T Saturation1_UpperSat;   // Computed Parameter: Saturation1_UpperSat
                                        //  Referenced by: '<S6>/Saturation1'

    real32_T Saturation1_LowerSat;   // Computed Parameter: Saturation1_LowerSat
                                        //  Referenced by: '<S6>/Saturation1'

    real32_T Gain1_Gain;               // Computed Parameter: Gain1_Gain
                                          //  Referenced by: '<S6>/Gain1'

    real32_T Saturation7_UpperSat;   // Computed Parameter: Saturation7_UpperSat
                                        //  Referenced by: '<S3>/Saturation7'

    real32_T Saturation7_LowerSat;   // Computed Parameter: Saturation7_LowerSat
                                        //  Referenced by: '<S3>/Saturation7'

    real32_T Gain_Gain_j;              // Computed Parameter: Gain_Gain_j
                                          //  Referenced by: '<S7>/Gain'

    real32_T Integrator_gainval;       // Computed Parameter: Integrator_gainval
                                          //  Referenced by: '<S42>/Integrator'

    real32_T Saturation10_UpperSat; // Computed Parameter: Saturation10_UpperSat
                                       //  Referenced by: '<S3>/Saturation10'

    real32_T Saturation10_LowerSat; // Computed Parameter: Saturation10_LowerSat
                                       //  Referenced by: '<S3>/Saturation10'

    real32_T Delay_InitialCondition;
                                   // Computed Parameter: Delay_InitialCondition
                                      //  Referenced by: '<S6>/Delay'

    real32_T antiWU_Gain_Gain;         // Computed Parameter: antiWU_Gain_Gain
                                          //  Referenced by: '<S6>/antiWU_Gain'

    real32_T Delay1_InitialCondition;
                                  // Computed Parameter: Delay1_InitialCondition
                                     //  Referenced by: '<S6>/Delay1'

    real32_T antiWU_Gain1_Gain;        // Computed Parameter: antiWU_Gain1_Gain
                                          //  Referenced by: '<S6>/antiWU_Gain1'

    uint16_T Saturation_UpperSat_d; // Computed Parameter: Saturation_UpperSat_d
                                       //  Referenced by: '<S1>/Saturation'

    uint16_T Saturation_LowerSat_i; // Computed Parameter: Saturation_LowerSat_i
                                       //  Referenced by: '<S1>/Saturation'

    uint16_T Constant_Value;           // Computed Parameter: Constant_Value
                                          //  Referenced by: '<S1>/Constant'

    uint16_T Switch_Threshold;         // Computed Parameter: Switch_Threshold
                                          //  Referenced by: '<S1>/Switch'

  };

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  px4_AlgorithmModelClass();

  // Destructor
  ~px4_AlgorithmModelClass();

  // Root-level structure-based inputs set method

  // Root inports set method
  void setExternalInputs(const ExtU_Second_att_control_codeb_T
    * pExtU_Second_att_control_codeb_T)
  {
    Second_att_control_codeblock__U = *pExtU_Second_att_control_codeb_T;
  }

  // Root-level structure-based outputs get method

  // Root outports get method
  const px4_AlgorithmModelClass::ExtY_Second_att_control_codeb_T
    & getExternalOutputs() const
  {
    return Second_att_control_codeblock__Y;
  }

  // private data and function members
 private:
  // Tunable parameters
  static P_Second_att_control_codebloc_T Second_att_control_codeblock__P;

  // Block signals
  B_Second_att_control_codebloc_T Second_att_control_codeblock__B;

  // Block states
  DW_Second_att_control_codeblo_T Second_att_control_codeblock_DW;

  // External inputs
  ExtU_Second_att_control_codeb_T Second_att_control_codeblock__U;

  // External outputs
  ExtY_Second_att_control_codeb_T Second_att_control_codeblock__Y;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S8>/Data Type Duplicate' : Unused code path elimination
//  Block '<S9>/Data Type Duplicate' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Second_att_control_codeblock_fly'
//  '<S1>'   : 'Second_att_control_codeblock_fly/Attitude Control1'
//  '<S2>'   : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl'
//  '<S3>'   : 'Second_att_control_codeblock_fly/Attitude Control1/InputConditioning'
//  '<S4>'   : 'Second_att_control_codeblock_fly/Attitude Control1/pwm_out1'
//  '<S5>'   : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Roll-pitch_Angle_to_rate'
//  '<S6>'   : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Roll-pitch_rate_controller'
//  '<S7>'   : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control'
//  '<S8>'   : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Roll-pitch_rate_controller/Discrete Derivative'
//  '<S9>'   : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Roll-pitch_rate_controller/Discrete Derivative1'
//  '<S10>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller'
//  '<S11>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Anti-windup'
//  '<S12>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/D Gain'
//  '<S13>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Filter'
//  '<S14>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Filter ICs'
//  '<S15>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/I Gain'
//  '<S16>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Ideal P Gain'
//  '<S17>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Ideal P Gain Fdbk'
//  '<S18>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Integrator'
//  '<S19>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Integrator ICs'
//  '<S20>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/N Copy'
//  '<S21>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/N Gain'
//  '<S22>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/P Copy'
//  '<S23>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Parallel P Gain'
//  '<S24>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Reset Signal'
//  '<S25>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Saturation'
//  '<S26>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Saturation Fdbk'
//  '<S27>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Sum'
//  '<S28>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Sum Fdbk'
//  '<S29>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Tracking Mode'
//  '<S30>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Tracking Mode Sum'
//  '<S31>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Tsamp - Integral'
//  '<S32>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Tsamp - Ngain'
//  '<S33>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/postSat Signal'
//  '<S34>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/preSat Signal'
//  '<S35>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Anti-windup/Passthrough'
//  '<S36>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/D Gain/Disabled'
//  '<S37>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Filter/Disabled'
//  '<S38>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Filter ICs/Disabled'
//  '<S39>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/I Gain/Internal Parameters'
//  '<S40>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Ideal P Gain/Passthrough'
//  '<S41>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
//  '<S42>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Integrator/Discrete'
//  '<S43>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Integrator ICs/Internal IC'
//  '<S44>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/N Copy/Disabled wSignal Specification'
//  '<S45>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/N Gain/Disabled'
//  '<S46>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/P Copy/Disabled'
//  '<S47>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Parallel P Gain/Internal Parameters'
//  '<S48>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Reset Signal/Disabled'
//  '<S49>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Saturation/Passthrough'
//  '<S50>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Saturation Fdbk/Disabled'
//  '<S51>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Sum/Sum_PI'
//  '<S52>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Sum Fdbk/Disabled'
//  '<S53>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Tracking Mode/Disabled'
//  '<S54>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Tracking Mode Sum/Passthrough'
//  '<S55>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Tsamp - Integral/Passthrough'
//  '<S56>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/Tsamp - Ngain/Passthrough'
//  '<S57>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/postSat Signal/Forward_Path'
//  '<S58>'  : 'Second_att_control_codeblock_fly/Attitude Control1/AttitudeControl/Yaw_rate_control/Discrete PID Controller/preSat Signal/Forward_Path'

#endif                        // RTW_HEADER_Second_att_control_codeblock_fly_h_

//
// File trailer for generated code.
//
// [EOF]
//
