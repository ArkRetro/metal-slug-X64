/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c616c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c616c(void)

{
  short sVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  undefined4 *unaff_A6;
  undefined8 uVar6;

  candidate_1b1dc();
  unaff_A6[0x1e] = 0;
  unaff_A6[0x1f] = 0;
  unaff_A6[0x20] = 0;
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0xffff;
  unaff_A6[0x21] = 0;
  candidate_4256();
  *unaff_A6 = 0xc61a4;
  candidate_1abc8();
  sVar1 = *(short *)(unaff_A6[3] + 0x8c);
  *(char *)((int)unaff_A6 + 0x97) = (char)sVar1;
  if (sVar1 != *(short *)((int)unaff_A6 + 0x8e)) {
    iVar2 = *(int *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x96) << 2) + 0xe7c2e);
    sVar3 = *(short *)((int)unaff_A6 + 0x8e);
    *(short *)((int)unaff_A6 + 0x8e) = sVar1;
    if (sVar3 == -1) {
      sVar3 = *(short *)(iVar2 + (short)(sVar1 << 2));
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
        sVar3 = -sVar3;
      }
      *(short *)(unaff_A6 + 10) = sVar3 + *(short *)(unaff_A6 + 10);
      *(short *)(unaff_A6 + 0xb) =
           *(short *)(iVar2 + 2 + (int)(short)(sVar1 << 2)) + *(short *)(unaff_A6 + 0xb);
    }
    else {
      sVar3 = sVar3 << 2;
      sVar4 = *(short *)(iVar2 + (short)(sVar1 << 2));
      sVar5 = *(short *)(iVar2 + sVar3);
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
        sVar4 = -sVar4;
        sVar5 = -sVar5;
      }
      *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) - sVar5;
      *(short *)(unaff_A6 + 10) = sVar4 + *(short *)(unaff_A6 + 10);
      sVar1 = *(short *)(iVar2 + 2 + (int)(short)(sVar1 << 2));
      *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) - *(short *)(iVar2 + 2 + (int)sVar3);
      *(short *)(unaff_A6 + 0xb) = sVar1 + *(short *)(unaff_A6 + 0xb);
    }
  }
  unaff_A6[10] = unaff_A6[10] - unaff_A6[0x1e];
  unaff_A6[0xb] = unaff_A6[0xb] - unaff_A6[0x1f];
  unaff_A6[0x21] =
       *(short *)(*(int *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x96) << 2) + 0xe7bc8) +
                 (int)(short)(*(short *)((int)unaff_A6 + 0x8e) * 2)) * 0x100 + unaff_A6[0x21];
  uVar6 = candidate_2e7c();
  iVar2 = (int)((ulonglong)uVar6 >> 0x20);
  unaff_A6[10] = iVar2 + unaff_A6[10];
  unaff_A6[0x1e] = iVar2;
  unaff_A6[0xb] = (int)uVar6 + unaff_A6[0xb];
  unaff_A6[0x1f] = (int)uVar6;
  candidate_c6bc4();
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)(unaff_A6[3] + 0x1e) + -1;
  candidate_427a();
  if (0x1fffff < (int)unaff_A6[0x21]) {
    *unaff_A6 = candidate_c62d4;
  }
  iVar2 = *(int *)(unaff_A6[3] + 0xc);
  if (((*(char *)(iVar2 + 0x97) == '\x06') || (*(char *)(iVar2 + 0x97) == '\x05')) ||
     (*(char *)(iVar2 + 0x97) == '\a')) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
