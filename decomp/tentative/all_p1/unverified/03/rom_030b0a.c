/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030b0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_30b0a(void)

{
  undefined2 uVar1;
  short sVar2;
  char cVar3;
  char cVar4;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar5;

  if (*(short *)(unaff_A6 + 0x20) == 0) {
    uVar1 = 0x30;
  }
  else {
    uVar1 = 0xb8;
  }
  *(undefined2 *)(unaff_A6 + 0xf) = uVar1;
  *(undefined2 *)(unaff_A6 + 0x10) = 400;
  candidate_301ae();
  candidate_302c2();
  if (*(short *)(unaff_A6 + 0x20) == 0) {
    uVar1 = 10;
  }
  else {
    uVar1 = 0x1b;
  }
  *(undefined2 *)(unaff_A6 + 10) = uVar1;
  *(undefined2 *)(unaff_A6 + 0xb) = 9;
  candidate_30162();
  sVar2 = _DAT_0010c600;
  cVar3 = DAT_0010c602;
  cVar4 = DAT_0010c603;
  if (*(short *)(unaff_A6 + 0x20) == 0) {
    sVar2 = _DAT_0010c5fc;
    cVar3 = DAT_0010c5fe;
    cVar4 = DAT_0010c5ff;
  }
  *(short *)((int)unaff_A6 + 0x82) = sVar2;
  *(short *)((int)unaff_A6 + 0x86) = sVar2;
  if (cVar3 != '\0') {
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 1;
    *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + 1;
  }
  if (cVar4 != '\0') {
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 2;
    *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + 1;
  }
  if (*(short *)((int)unaff_A6 + 0x82) == 0) {
    candidate_3035e();
  }
  else {
    func_0x0003032c();
  }
  if (*(short *)((int)unaff_A6 + 0x82) == 0) {
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x40;
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x80;
  }
  else {
    bVar5 = false;
    candidate_1aeb0();
    if (bVar5) {
      bVar5 = false;
      candidate_1aeb0();
      if (bVar5) {
        cVar3 = -1;
      }
      else {
        cVar3 = '\0';
      }
    }
    else {
      cVar3 = '\0';
    }
    if (cVar3 == '\0') {
      if (9 < *(short *)((int)unaff_A6 + 0x82)) {
        *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x10;
        *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x80;
      }
    }
    else {
      *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x80;
      if (_DAT_0010c5fc == _DAT_0010c600) {
        *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x20;
      }
      else {
        if (_DAT_0010c600 < _DAT_0010c5fc) {
          *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x10;
        }
        if (*(short *)(unaff_A6 + 0x20) != 0) {
          *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) ^ 0x10;
        }
      }
    }
  }
  cVar3 = candidate_18fea();
  if (cVar3 == '\0') {
    cVar3 = candidate_18fea();
    if (cVar3 != '\0') {
      *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 8;
    }
  }
  else {
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 4;
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 8;
    *(char *)((int)unaff_A6 + 0x8e) = cVar3;
  }
  candidate_40bc();
  *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A0;
  candidate_9ea8();
  candidate_4256();
  candidate_30374();
  candidate_30608();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x30d20;
  cVar3 = *(char *)(unaff_A6[3] + 0x61);
  *(char *)((int)unaff_A6 + 0x61) = cVar3;
  if (cVar3 != '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 1;
  }
  *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
  if (*(char *)(unaff_A6 + 0x18) != '\0') {
    if (*(char *)(unaff_A6 + 0x18) == -1) {
      *(undefined1 *)(unaff_A6 + 0x18) = 1;
      candidate_306d0();
      if (*(short *)((int)unaff_A6 + 0x82) < *(short *)(unaff_A6 + 0x21)) {
        candidate_303aa();
        candidate_3040e();
        candidate_3040e();
      }
      candidate_3040e();
      candidate_303de();
    }
    else if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
      candidate_30608();
    }
  }
  if ((*(short *)((int)unaff_A6 + 0x7e) < 1) &&
     (*(undefined2 *)((int)unaff_A6 + 0x7e) = 0, _DAT_0010c600 == 0 && _DAT_0010c5fc == 0)) {
    if (*(short *)((int)unaff_A6 + 0x86) != 0) {
      candidate_303d8();
    }
    *unaff_A6 = candidate_30dba;
  }
  *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
  candidate_427a();
  return;
}
