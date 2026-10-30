/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08a264.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8a264(void)

{
  int iVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar4;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  bool bVar6;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xffff;
  candidate_1ad6e();
  *unaff_A6 = 0x8a27a;
  iVar1 = unaff_A6[3];
  if (*(char *)(iVar1 + 0x8e) != '\0') {
    return;
  }
  *(undefined1 *)((int)unaff_A6 + 0x15) = *(undefined1 *)(iVar1 + 0x15);
  uVar2 = *(ushort *)(iVar1 + 0x76);
  uVar3 = *(ushort *)((int)unaff_A6 + 0x6e);
  if (uVar2 != uVar3) {
    *(ushort *)((int)unaff_A6 + 0x6e) = uVar2;
    unaff_A6[0x14] =
         *(undefined4 *)
          (*(int *)((short)(uVar2 * 4) + 0xe52a6) +
          (int)(short)((*(byte *)(unaff_A6 + 0x1c) & 1) * 4));
  }
  uVar5 = uVar2 == uVar3 && uVar2 < uVar3;
  candidate_1abc8();
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    bVar6 = false;
    candidate_1aeb0();
    if (bVar6) {
      sVar4 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0xf);
      if ((*(byte *)(extraout_A0 + 0x15) & 1) == 0) {
        sVar4 = -sVar4;
      }
      if ((*(short *)(unaff_A6[3] + 0x76) == 2) || (0xf < sVar4)) {
        uVar2 = *(short *)(unaff_A6[3] + 0x76) * 2;
        bVar6 = CARRY2(uVar2,uVar2);
        (*(code *)((short)(*(short *)(unaff_A6[3] + 0x76) * 4) + 0x8a340))();
        if (bVar6) {
          uVar5 = 1;
          if (*(char *)(unaff_A6 + 0x1c) != '\0') {
            uVar5 = 2;
          }
          *(undefined1 *)(unaff_A6[3] + 0x89) = uVar5;
        }
      }
    }
  }
  return;
}
