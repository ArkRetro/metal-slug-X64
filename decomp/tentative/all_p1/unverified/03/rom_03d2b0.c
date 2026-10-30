/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03d2b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3d2b0(void)

{
  undefined4 uVar1;
  char cVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 extraout_D1w;
  undefined2 extraout_D1w_00;
  char unaff_D2b;
  short unaff_D7w;
  char *extraout_A0;
  char *extraout_A0_00;
  undefined4 extraout_A0_01;
  char *extraout_A0_02;
  code *extraout_A1;
  code *extraout_A1_00;
  code *pcVar5;
  code *extraout_A1_01;
  code *extraout_A1_02;
  undefined2 *unaff_A3;

  while( true ) {
    uVar4 = unaff_A3[1];
    uVar3 = candidate_3d1b8();
    if (unaff_D2b == '\0') {
      return;
    }
    unaff_A3[1] = uVar4;
    unaff_A3[2] = extraout_D1w;
    *unaff_A3 = uVar3;
    pcVar5 = extraout_A1;
    if ((unaff_D2b != '%') || (cVar2 = *extraout_A0, cVar2 == '%')) break;
    candidate_3d188();
    if (unaff_D7w == 0) {
      __m68k_trap(0xf);
    }
    unaff_D2b = *extraout_A0_00;
    if (unaff_D7w != unaff_A3[0xb]) {
      if ((char)unaff_A3[0xb] == '\0') {
        uVar1 = **(undefined4 **)(unaff_A3 + 7);
        *(undefined4 **)(unaff_A3 + 7) = *(undefined4 **)(unaff_A3 + 7) + 1;
        *(undefined4 *)(unaff_A3 + 9) = uVar1;
      }
      if (unaff_D2b == 's') {
        unaff_A3[0xb] = unaff_A3[0xb] + 1;
        candidate_3d1b8();
        unaff_A3[2] = extraout_D1w_00;
        unaff_A3[1] = CONCAT11((char)((ushort)uVar4 >> 8),-(cVar2 == '0'));
        uVar4 = (*extraout_A1_01)();
        *(char **)(unaff_A3 + 9) = extraout_A0_02;
        *unaff_A3 = uVar4;
        if (*extraout_A0_02 == '\0') {
          unaff_A3[0xb] = unaff_D7w;
        }
        return;
      }
      if (unaff_D2b == 'x') {
        unaff_A3[0xb] = unaff_A3[0xb] + 1;
        candidate_3d12c();
        uVar4 = (*extraout_A1_02)();
        *unaff_A3 = uVar4;
        return;
      }
      __m68k_trap(0xf);
      pcVar5 = extraout_A1_00;
      break;
    }
    unaff_A3[0xb] = 0;
    *(char **)(unaff_A3 + 3) = extraout_A0_00 + 1;
  }
  uVar4 = (*pcVar5)();
  *unaff_A3 = uVar4;
  *(undefined4 *)(unaff_A3 + 3) = extraout_A0_01;
  return;
}
