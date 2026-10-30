/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0871aa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_871aa(void)

{
  uint uVar1;
  char cVar3;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  bVar4 = *(byte *)((int)unaff_A6 + 0x5a) < 7;
  if (*(byte *)((int)unaff_A6 + 0x5a) == 7) {
    candidate_29c6();
    iVar2 = candidate_2e7c();
    unaff_A6[0xc] = iVar2 + unaff_A6[0xc];
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      candidate_87c0e();
      candidate_120f8();
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
      candidate_4256();
      candidate_9f88();
      candidate_9ea8();
      *unaff_A6 = 0x87362;
      unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
      candidate_143dc();
      candidate_427a();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
        candidate_4256();
        *unaff_A6 = 0x8739c;
        uVar1 = unaff_A6[0xc];
        uVar5 = uVar1 < (uint)((int)unaff_A6[0xc] >> 3);
        unaff_A6[0xc] = uVar1 - ((int)unaff_A6[0xc] >> 3);
        candidate_143dc();
        candidate_427a();
        if ((bool)uVar5) {
          *unaff_A6 = candidate_873c0;
        }
      }
    }
    else {
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
      candidate_120f8();
      candidate_4256();
      candidate_9f88();
      candidate_9ea8();
      *unaff_A6 = 0x872fa;
      uVar1 = unaff_A6[0xc];
      uVar5 = uVar1 < (uint)((int)unaff_A6[0xc] >> 3);
      unaff_A6[0xc] = uVar1 - ((int)unaff_A6[0xc] >> 3);
      candidate_143dc();
      candidate_427a();
      if ((bool)uVar5) {
        *unaff_A6 = candidate_873c0;
      }
    }
  }
  else if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) ||
          ((candidate_1b206(), bVar4 && (cVar3 = candidate_29c6(), cVar3 < '\0')))) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
    candidate_120f8();
    candidate_1b1dc();
    candidate_9f88();
    candidate_9ea8();
    candidate_4256();
    *unaff_A6 = 0x87272;
    uVar5 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_873c0;
    }
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
    candidate_120f8();
    candidate_1b1dc();
    candidate_4256();
    *unaff_A6 = 0x87206;
    uVar5 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_873c0;
    }
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
