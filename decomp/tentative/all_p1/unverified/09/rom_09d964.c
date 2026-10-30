/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09d964.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9d964(void)

{
  short sVar1;
  char cVar2;
  int iVar3;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar4;
  bool bVar5;

  candidate_1436a();
  candidate_9f0fa();
  if ((bool)in_CF) {
    candidate_9f196();
    if ((bool)in_CF) {
      *unaff_A6 = candidate_9e25a;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x97) = *(byte *)((int)unaff_A6 + 0x97) ^ 1;
      *(undefined1 *)((int)unaff_A6 + 0x92) = 0xf;
      *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
    }
  }
  candidate_9efc8();
  *(char *)((int)unaff_A6 + 0x92) = *(char *)((int)unaff_A6 + 0x92) + -1;
  uVar4 = false;
  if (*(char *)((int)unaff_A6 + 0x92) < '\x01') {
    *(byte *)((int)unaff_A6 + 0x97) = *(byte *)((int)unaff_A6 + 0x97) ^ 1;
    *(undefined1 *)((int)unaff_A6 + 0x92) = 0xf;
    iVar3 = -*(int *)((int)unaff_A6 + 0x62);
    *(int *)((int)unaff_A6 + 0x62) = iVar3;
    uVar4 = iVar3 != 0;
  }
  candidate_427a();
  candidate_1af9a();
  if (!(bool)uVar4) {
    candidate_9f0bc();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_9e00a;
      uVar4 = 0;
    }
    else {
      candidate_9f0fa();
      if ((bool)uVar4) {
        candidate_9f196();
        if ((bool)uVar4) {
          *unaff_A6 = candidate_9e25a;
          uVar4 = 0;
        }
      }
      else {
        candidate_9f2bc();
        if ((bool)uVar4) {
          *unaff_A6 = candidate_9dd12;
          uVar4 = 0;
        }
        candidate_9f156();
        if ((bool)uVar4) {
          *unaff_A6 = candidate_9e250;
        }
        uVar4 = false;
        if (*(char *)(unaff_A6 + 0x24) < '\0') {
          candidate_9f250();
          if ((bool)uVar4) {
            *unaff_A6 = candidate_9db44;
            uVar4 = 0;
          }
        }
        else {
          cVar2 = *(char *)(unaff_A6 + 0x24);
          uVar4 = cVar2 == '\0';
          *(char *)(unaff_A6 + 0x24) = cVar2 + -1;
        }
        candidate_9f280();
        if ((bool)uVar4) {
          *unaff_A6 = candidate_9ddce;
          uVar4 = 0;
          if ((*(byte *)(unaff_A6 + 0x26) & 0x40) != 0) {
            *(byte *)(unaff_A6 + 0x26) = *(byte *)(unaff_A6 + 0x26) | 0x80;
            candidate_9f432();
            *unaff_A6 = candidate_9db44;
            uVar4 = 0;
          }
        }
      }
    }
    candidate_9f348();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_9e062;
      uVar4 = 0;
    }
    candidate_9f314();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_9e122;
      uVar4 = 0;
    }
  }
  candidate_9f208();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9e040;
    uVar4 = 0;
  }
  candidate_9f1c8();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9dc5c;
  }
  if (((*(byte *)(unaff_A6 + 0x26) & 4) != 0) &&
     (*(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1,
     *(short *)((int)unaff_A6 + 0x8a) < 1)) {
    *unaff_A6 = candidate_9de84;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_9e34c;
  }
  sVar1 = *(short *)(unaff_A6 + 0x22);
  bVar5 = sVar1 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar1 + -1;
  candidate_4c24();
  if (bVar5) {
    candidate_9f53e();
    candidate_9f5fa();
    cVar2 = *(char *)((int)unaff_A6 + 0x5a);
    if (cVar2 == '\x02') {
      *unaff_A6 = candidate_9e8c2;
    }
    if (cVar2 == '\x01') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar2 == '\f') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar2 == '\r') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar2 == '\t') {
      *unaff_A6 = candidate_9e920;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_9eab8;
    }
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_9ec34;
  }
  bVar5 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
