/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039260.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_39260(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_4256();
  *unaff_A6 = 0x39276;
  candidate_1ac38();
  candidate_427a();
  unaff_A6[0xc] = unaff_A6[0xc] + -0x5000;
  unaff_A6[0xd] = unaff_A6[0xd] + -0xa00;
  if (*(short *)(unaff_A6 + 0xf) < -0x5f) {
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
