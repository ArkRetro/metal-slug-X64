/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087716.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87716(void)

{
  ushort uVar3;
  int iVar1;
  uint uVar2;
  short sVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  uVar3 = candidate_29c6();
  iVar1 = *(int *)((short)((uVar3 & 3) << 2) + 0x87d1e);
  sVar4 = (*(byte *)((int)unaff_A6 + 0x89) & 7) * 0xc;
  unaff_A6[0xd] = *(undefined4 *)(iVar1 + sVar4);
  *(undefined4 *)((int)unaff_A6 + 0x72) = *(undefined4 *)(iVar1 + 4 + (int)sVar4);
  iVar1 = *(int *)(iVar1 + 8 + (int)sVar4);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = -iVar1;
  }
  unaff_A6[0xc] = iVar1;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  candidate_4256();
  uVar2 = candidate_29c6();
  if ((uVar2 & 1) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x80;
  }
  *unaff_A6 = 0x877a0;
  uVar5 = 0;
  candidate_87892();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_877de;
    uVar5 = 0;
    if (*(char *)(unaff_A6 + 0x22) == '\0') {
      *unaff_A6 = candidate_41ac;
      uVar5 = 0;
    }
  }
  candidate_19504();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
