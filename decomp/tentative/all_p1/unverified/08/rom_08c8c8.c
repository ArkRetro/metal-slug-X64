/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08c8c8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8c8c8(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_8c92c();
  if ((bool)in_CF) {
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
  if (*(short *)(unaff_A6 + 0x1f) == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x7a) = 0xf;
    *unaff_A6 = candidate_8c8ec;
  }
  return;
}
