/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a502c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a502c(void)

{
  byte bVar1;
  short sVar2;
  byte bVar3;
  undefined2 uVar4;
  ushort uVar5;
  char cVar6;
  int unaff_A5;
  undefined4 *unaff_A6;

  uVar4 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar4;
  uVar5 = candidate_29c6();
  *(short *)((int)unaff_A6 + 0x86) = (uVar5 & 0x1f) + *(short *)((int)unaff_A6 + 0x86);
  candidate_4256();
  candidate_1b1dc();
  *unaff_A6 = 0xa5066;
  bVar1 = *(byte *)(unaff_A6[3] + 0x8b);
  bVar3 = '\x01' << ((byte)(*(char *)((int)unaff_A6 + 0x8d) + 2) & 7);
  *(byte *)(unaff_A6[3] + 0x8b) = bVar1 & ~bVar3;
  if ((bVar1 & bVar3) == 0) {
    sVar2 = *(short *)((int)unaff_A6 + 0x86) + -1;
    *(short *)((int)unaff_A6 + 0x86) = sVar2;
    if (-1 < sVar2) {
      candidate_1abc8();
      candidate_427a();
      goto LAB_000a5166;
    }
    cVar6 = candidate_a535e();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      cVar6 = -(cVar6 + -3);
    }
    *(char *)((int)unaff_A6 + 0x8a) = cVar6 * '\x02';
    *(undefined4 *)((int)unaff_A6 + 0x7e) = 0xa50b6;
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 4;
    *(undefined4 *)((int)unaff_A6 + 0x7e) = 0xa5190;
  }
  candidate_4256();
  *(char *)(unaff_A6 + 0x22) = *(char *)((int)unaff_A6 + 0x89) * '\x02';
  *unaff_A6 = candidate_a5122;
  if ((*(ushort *)(unaff_A5 + -0x7ff2) & 3) == 0) {
    bVar1 = *(char *)((int)unaff_A6 + 0x8a) - *(char *)(unaff_A6 + 0x22);
    if (bVar1 == 0) {
      *(byte *)((int)unaff_A6 + 0x89) = *(byte *)(unaff_A6 + 0x22) >> 1;
                    /* WARNING: Could not recover jumptable at 0x000a5164. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)((int)unaff_A6 + 0x7e))();
      return;
    }
    *(byte *)(unaff_A6 + 0x22) =
         (-((short)((ushort)bVar1 << 8) < 0) & 0xfeU) + 1 + *(char *)(unaff_A6 + 0x22);
  }
  candidate_1abc8();
  candidate_427a();
LAB_000a5166:
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
    *unaff_A6 = FUN_000a52d6;
  }
  return;
}
