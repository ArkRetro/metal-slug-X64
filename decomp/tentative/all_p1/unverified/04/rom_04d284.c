/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d284.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4d284(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  unaff_A6[0xd] = unaff_A6[0xd] + 0x4000;
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 8);
  candidate_1ac38();
  candidate_427a();
  if (0x1df < (short)*(ushort *)(unaff_A6 + 0x10)) {
    *unaff_A6 = candidate_4d2ce;
  }
  uVar3 = *(ushort *)(unaff_A6 + 0x10) < 0x1e0;
  candidate_4d608();
  candidate_4e092();
  if ((bool)uVar3) {
    *(short *)(unaff_A6[3] + 0x86) = *(short *)(unaff_A6[3] + 0x86) + -1;
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
