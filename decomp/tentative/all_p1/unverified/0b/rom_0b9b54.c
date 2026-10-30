/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b9b54.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b9b54(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if (*(short *)(unaff_A5 + 0x46e0) != 0) {
    *unaff_A6 = candidate_b9b6c;
  }
  uVar3 = 0;
  candidate_1abc8();
  candidate_1945c();
  if ((bool)uVar3) {
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
