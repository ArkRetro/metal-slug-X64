/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0be8a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_be8a2(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  DAT_0010c716 = '\0';
  *unaff_A6 = 0xbe8ae;
  if (DAT_0010c716 != '\0') {
    candidate_40bc();
    *(undefined4 *)(extraout_A0 + 0x38) = 0x10e968;
    *(undefined4 *)(extraout_A0 + 0x28) = 0x11e00000;
    *(undefined4 *)(extraout_A0 + 0x2c) = 0x380000;
    *(undefined4 *)(extraout_A0 + 0x6e) = 0xbe89a;
    *(undefined2 *)(extraout_A0 + 0x72) = 0x1220;
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
