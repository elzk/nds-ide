//---------------------------------------------------------------------------*
//                                                                           *
//                        File 'goil_obj_appmode.h'                          *
//                        Generated by version 2.0.0                         *
//                       july 7th, 2010, at 17h16'31"                        *
//                                                                           *
//---------------------------------------------------------------------------*

//--- START OF USER ZONE 1


//--- END OF USER ZONE 1

#ifndef goil_obj_appmode_DEFINED
#define goil_obj_appmode_DEFINED

#include <string.h>

//---------------------------------------------------------------------------*

#include "galgas/GGS__header.h"
#include "goil_lexique.h"

//---------------------------------------------------------------------------*

// Include imported semantics
#include "goil_options.h"
#include "goil_semantics.h"

//--- START OF USER ZONE 2


//--- END OF USER ZONE 2

//---------------------------------------------------------------------------*
//                                                                           *
//                          Class Predeclarations                            *
//                                                                           *
//---------------------------------------------------------------------------*


//---------------------------------------------------------------------------*
//                                                                           *
//                Parser class goil_obj_appmode declaration                  *
//                                                                           *
//---------------------------------------------------------------------------*

class goil_obj_appmode {
  public : virtual ~goil_obj_appmode (void) {}

  protected : virtual void nt_appmode_ (C_Lexique_goil_5F_lexique &) = 0 ;

  protected : virtual void nt_appmode_parse (C_Lexique_goil_5F_lexique &) = 0 ;

  protected : virtual void nt_description_ (C_Lexique_goil_5F_lexique &,
                                GGS_lstring  &) = 0 ;

  protected : virtual void nt_description_parse (C_Lexique_goil_5F_lexique &) = 0 ;

  protected : virtual void nt_free_5F_field_ (C_Lexique_goil_5F_lexique &,
                                GGS_ident_map  &) = 0 ;

  protected : virtual void nt_free_5F_field_parse (C_Lexique_goil_5F_lexique &) = 0 ;

  protected : void rule_goil_5F_obj_5F_appmode_appmode_i0_ (C_Lexique_goil_5F_lexique &) ;

  protected : virtual PMSInt16 select_goil_5F_obj_5F_appmode_0 (C_Lexique_goil_5F_lexique &) = 0 ;

  protected : void rule_goil_5F_obj_5F_appmode_appmode_i0_parse (C_Lexique_goil_5F_lexique & inLexique) ;

} ;

//---------------------------------------------------------------------------*

#endif
