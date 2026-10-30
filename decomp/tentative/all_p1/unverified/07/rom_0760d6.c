/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0760d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_760d6(void)

{
  int iVar1;
  short sVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  candidate_9f88();
  candidate_9ea8();
  candidate_9ea8();
  candidate_4520();
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar3;
  candidate_4256();
  candidate_1ad6e();
  *unaff_A6 = 0x76136;
  uVar5 = 0;
  candidate_768ae();
  candidate_7693a();
  candidate_768f4();
  candidate_1abc8();
  iVar1 = unaff_A6[3];
  if ((*(byte *)(iVar1 + 0x84) & 2) == 0) {
    if ((*(byte *)(iVar1 + 0x84) & 4) == 0) {
      if (((*(byte *)(iVar1 + 0x84) & 1) != 0) &&
         (uVar5 = *(byte *)(unaff_A6 + 0x1c) < 0x10, '\x10' < (char)*(byte *)(unaff_A6 + 0x1c))) {
        sVar2 = *(short *)((int)unaff_A6 + 0x1e);
        uVar5 = sVar2 == 0;
        *(short *)((int)unaff_A6 + 0x1e) = sVar2 + -1;
      }
    }
    else {
      sVar2 = *(short *)((int)unaff_A6 + 0x1e);
      uVar5 = sVar2 == 0;
      *(short *)((int)unaff_A6 + 0x1e) = sVar2 + -1;
    }
    candidate_427a();
  }
  candidate_769b2();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_761c0;
  }
  bVar4 = *(char *)(unaff_A6[3] + 0x60) != -1;
  if (!bVar4) {
    *unaff_A6 = 0x765d4;
  }
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = 0x765d4;
  }
  return;
}
