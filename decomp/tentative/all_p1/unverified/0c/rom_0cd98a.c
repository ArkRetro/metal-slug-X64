/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd98a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd98a(void)

{
  short sVar1;
  ushort uVar2;
  byte bVar4;
  short sVar3;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_1ad6e();
  bVar4 = candidate_29c6();
  *(byte *)((int)unaff_A6 + 0x85) = bVar4 & 7;
  *(undefined2 *)(unaff_A6 + 0x26) = 0;
  *unaff_A6 = 0xcd9ac;
  candidate_1abc8();
  if (*(char *)(unaff_A6[3] + 0x60) == '\0') {
    *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + 1;
    uVar2 = *(ushort *)(unaff_A6 + 0x26) & 3;
    *(ushort *)(unaff_A6 + 0x26) = uVar2;
    if (uVar2 == 0) {
      uVar2 = *(ushort *)(unaff_A6[3] + 0x6e);
      *(ushort *)((int)unaff_A6 + 0x6e) = uVar2;
      sVar3 = (uVar2 & 0x1f) << 2;
      sVar1 = *(short *)(sVar3 + 0xcec1e);
      *(undefined2 *)(unaff_A6 + 10) = *(undefined2 *)(sVar3 + 0xcec1c);
      *(short *)(unaff_A6 + 0xb) = sVar1 + 4;
      candidate_40bc();
      *(undefined1 *)(extraout_A0 + 0x85) = *(undefined1 *)((int)unaff_A6 + 0x85);
      *(undefined2 *)(extraout_A0 + 0x6e) = *(undefined2 *)((int)unaff_A6 + 0x6e);
    }
  }
  bVar5 = false;
  candidate_409a();
  if (!bVar5) {
    *unaff_A6 = 0xcd7ee;
  }
  if (*(int *)unaff_A6[3] == 0xcd626) {
    *unaff_A6 = 0xcd7ee;
  }
  return;
}
