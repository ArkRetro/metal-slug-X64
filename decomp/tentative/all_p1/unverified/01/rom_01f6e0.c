/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01f6e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1f6e0(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0x14;
  *unaff_A6 = 0x1f6f8;
  uVar4 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_1f7ea;
  }
  sVar3 = *(short *)(unaff_A6 + 0x1c) + -1;
  *(short *)(unaff_A6 + 0x1c) = sVar3;
  if (sVar3 == 0) {
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
