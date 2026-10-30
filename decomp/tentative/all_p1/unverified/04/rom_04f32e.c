/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04f32e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4f32e(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined1 *)(unaff_A6 + 0x23) = *(undefined1 *)(unaff_A6[3] + 0x8c);
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_4f960();
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
  candidate_4f94c();
  return;
}
