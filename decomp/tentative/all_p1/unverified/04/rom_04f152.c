/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04f152.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4f152(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  unaff_A6[0x14] = 0xe10f6;
  *unaff_A6 = 0x4f180;
  *(undefined1 *)(unaff_A6 + 0x23) = *(undefined1 *)(unaff_A6[3] + 0x8c);
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4f1c8;
    uVar3 = 0;
  }
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
