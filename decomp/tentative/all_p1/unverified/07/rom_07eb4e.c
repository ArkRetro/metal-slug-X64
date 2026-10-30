/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07eb4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7eb4e(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  candidate_9ea8();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_1b1dc();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  sVar3 = candidate_260c8();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    sVar3 = -sVar3;
  }
  unaff_A6[0xc] = (int)sVar3 << 8;
  unaff_A6[0x1f] = 0;
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 100;
  unaff_A6[0x20] = 0xfffe0000;
  *unaff_A6 = 0x7ebcc;
  bVar4 = false;
  candidate_7eeee();
  if (bVar4) {
    unaff_A6[0xd] = 0;
    candidate_4256();
    *unaff_A6 = 0x7ebee;
    func_0x000143e4();
  }
  candidate_427a();
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    *unaff_A6 = candidate_7ec5a;
  }
  uVar5 = false;
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar5 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_7ec5a;
      uVar5 = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_7ec5a;
    uVar5 = 0;
  }
  candidate_1945c();
  if ((bool)uVar5) {
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
