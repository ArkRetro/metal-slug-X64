/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x097fe8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void FUN_00097fe8(void)

{
  char cVar1;
  short sVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  undefined4 *unaff_A6;
  bool bVar6;
  bool bVar7;

  bVar7 = false;
  candidate_40bc();
  if (bVar7) {
    candidate_1b220();
  }
  candidate_9849c();
  do {
    if ((*(short *)((int)unaff_A6 + 0x8a) != 0) && (*(char *)((int)unaff_A6 + 0x8d) == '\0')) {
      *unaff_A6 = 0x98114;
      if ((*(byte *)(unaff_A6 + 0x23) & 8) != 0) goto LAB_0009816e;
      candidate_98750();
      candidate_9861e();
      if ((*(char *)((int)unaff_A6 + 0x8f) + 1U & 0xf) < 3) {
        *unaff_A6 = candidate_98142;
      }
      goto LAB_0009842c;
    }
    if (*(char *)((int)unaff_A6 + 0x8d) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *(undefined2 *)(unaff_A6 + 0x20) = *(undefined2 *)((int)unaff_A6 + 0x82);
    *unaff_A6 = 0x9805a;
    if (((*(char *)((int)unaff_A6 + 0x8d) != '\0') && (*(int *)((int)unaff_A6 + 0x6e) != 0)) &&
       (sVar2 = *(short *)(unaff_A6 + 0x20) + -1, *(short *)(unaff_A6 + 0x20) = sVar2, sVar2 < 0)) {
      *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) | 0x10;
    }
    if ((*(byte *)(unaff_A6 + 0x23) & 0x10) == 0) {
      if ((*(byte *)(unaff_A6 + 0x23) & 8) != 0) goto LAB_0009816e;
      candidate_9865c();
      candidate_98704();
      candidate_9861e();
      goto LAB_0009842c;
    }
    bVar7 = false;
    candidate_40bc();
    *(char *)(unaff_A6 + 0x18) = -!bVar7;
    *(char *)((int)unaff_A6 + 0x8d) = -!bVar7;
    *(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1;
    *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) & 0xef;
    if (*(char *)((int)unaff_A6 + 0x8d) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x980f0;
  } while (*(char *)(unaff_A6 + 0x18) != '\0');
  if ((*(byte *)(unaff_A6 + 0x23) & 8) == 0) {
    candidate_98750();
    candidate_9861e();
  }
  else {
LAB_0009816e:
    if (*(char *)((int)unaff_A6 + 0x8d) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    sVar2 = -0x800;
    sVar5 = 0x800;
    if (0x9f < *(short *)(unaff_A6 + 0xf)) {
      sVar2 = 0x800;
      sVar5 = -0x800;
    }
    unaff_A6[0xc] = sVar2 * 0x100 + unaff_A6[0xc];
    unaff_A6[0xd] = unaff_A6[0xd] + -0x40000;
    uVar3 = candidate_29c6();
    uVar4 = candidate_29c6();
    *(ushort *)((int)unaff_A6 + 0x72) = (uVar3 & 0x1ff) + sVar5;
    *(ushort *)(unaff_A6 + 0x1d) = (uVar4 & 0x1ff) + 0x200;
    *unaff_A6 = candidate_981ec;
    candidate_98770();
    candidate_9861e();
  }
LAB_0009842c:
  if ((*(short *)(unaff_A6 + 0x21) != 0) &&
     (sVar2 = *(short *)(unaff_A6 + 0x21) + -1, *(short *)(unaff_A6 + 0x21) = sVar2, sVar2 == 0)) {
    *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) | 8;
  }
  bVar7 = false;
  if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x91);
    bVar7 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x91) = cVar1 + -1;
  }
  candidate_4c24();
  if (bVar7) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) | 3;
  }
  uVar3 = *(ushort *)((int)unaff_A6 + 0x4e);
  uVar4 = *(ushort *)(unaff_A6 + 0x1e);
  bVar7 = uVar3 >= uVar4;
  if (!bVar7 || uVar3 == uVar4) {
    *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) | 3;
  }
  bVar6 = (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0;
  if (!bVar6) {
    *unaff_A6 = candidate_981f8;
  }
  bVar6 = bVar6 && ((bVar7 && uVar3 != uVar4) && uVar3 < uVar4);
  candidate_1945c();
  if (bVar6) {
    *unaff_A6 = 0x9841e;
  }
  return;
}
