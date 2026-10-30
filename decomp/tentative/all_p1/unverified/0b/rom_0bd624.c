/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bd624.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bd624(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(unaff_A6 + 0xb);
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0x10000;
  candidate_120f8();
  *unaff_A6 = 0xbd656;
  candidate_1ac38();
  candidate_427a();
  if (0x8f < (short)(*(short *)(unaff_A6 + 0xb) - *(short *)((int)unaff_A6 + 0x6e))) {
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
