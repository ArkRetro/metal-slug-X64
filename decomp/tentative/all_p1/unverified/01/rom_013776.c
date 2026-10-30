/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013776.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_13776(void)

{
  uint uVar1;
  int unaff_D2;
  int unaff_D3;
  int iVar2;
  char unaff_D6b;
  undefined4 unaff_D7;

  uRam0010c014 = 0;
  uVar1 = candidate_12938();
  if ((short)unaff_D7 == 0x38) {
    uRam0010c014 = 1;
  }
  uVar1 = uVar1 << 0x10 | uVar1 >> 0x10;
  iRam0010c028 = 0;
  uRam0010c016 = unaff_D7;
  if (unaff_D2 != 0) goto LAB_000137da;
joined_r0x000137ac:
  if (unaff_D3 == 0) {
LAB_000137fe:
    if (uRam0010c016._2_2_ == 0x3c) {
      uRam0010c012 = uRam0010c012 | 0x200;
    }
    return uVar1;
  }
LAB_000137b0:
  uVar1 = candidate_1351a();
  if (unaff_D3 != 0) goto code_r0x000137ba;
  if (unaff_D2 != 0) goto LAB_000137da;
  goto LAB_000137fe;
code_r0x000137ba:
  if (unaff_D2 != 0) {
    iVar2 = unaff_D2;
    if (unaff_D2 < 0) {
      iVar2 = -unaff_D2;
    }
    iRam0010c028 = iVar2 + iRam0010c028;
    if ((-1 < iRam0010c028) || (unaff_D6b != '\0')) {
LAB_000137da:
      do {
        do {
          uVar1 = candidate_13214();
          if (unaff_D2 == 0) goto joined_r0x000137ac;
        } while (unaff_D3 == 0);
        iVar2 = unaff_D3;
        if (unaff_D3 < 0) {
          iVar2 = -unaff_D3;
        }
        iRam0010c028 = iRam0010c028 - iVar2;
      } while ((-1 < iRam0010c028) && (unaff_D6b == '\0'));
    }
  }
  goto LAB_000137b0;
}
