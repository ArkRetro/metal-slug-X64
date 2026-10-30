/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04cf56.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4cf56(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  *unaff_A6 = 0x4cf5c;
  uVar3 = 0;
  candidate_1abc8();
  candidate_4df02();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_4cf92;
  }
  if (*(short *)((int)unaff_A6 + 0x86) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x9f) = 0;
  }
  bVar4 = false;
  candidate_4d74a();
  if (bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
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
