/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x032aec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_32aec(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_8ccd4();
  candidate_427a();
  candidate_1945c();
  if ((bool)in_CF) {
    *(undefined1 *)(*(int *)((int)unaff_A6 + 0x76) + 0x81) = 0xff;
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
