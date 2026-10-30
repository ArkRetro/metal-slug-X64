/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b4e62.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b4e62(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined4 *)(unaff_A5 + 0x695c) = 0x20000;
  unaff_A6[0xc] = 0x20000;
  unaff_A6[0xd] = 0;
  func_0x000143e4();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0xf) < 0) {
    candidate_4192();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_140c0();
  return;
}
