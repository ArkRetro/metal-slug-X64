/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8334.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8334(void)

{
  char cVar1;
  ushort uVar2;
  undefined2 uVar3;
  short sVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  candidate_d9276();
  *(undefined1 *)(unaff_A6[3] + 0x81) = 0;
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + 1;
  uVar2 = *(ushort *)((int)unaff_A6 + 0x76) & 1;
  *(ushort *)((int)unaff_A6 + 0x76) = uVar2;
  if (uVar2 == 0) {
    candidate_2eb4();
    uVar3 = candidate_d9360();
    *(undefined2 *)(unaff_A6 + 0x1f) = uVar3;
    sVar4 = -1;
    if (*(ushort *)(unaff_A6 + 0x1f) == *(ushort *)((int)unaff_A6 + 0x6e)) {
      *(undefined1 *)(unaff_A6[3] + 0x81) = 1;
    }
    else {
      if (*(ushort *)((int)unaff_A6 + 0x6e) <= *(ushort *)(unaff_A6 + 0x1f)) {
        sVar4 = 1;
      }
      *(short *)((int)unaff_A6 + 0x6e) = sVar4 + *(short *)((int)unaff_A6 + 0x6e);
    }
  }
  candidate_427a();
  candidate_d92ec();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  uVar5 = false;
  if (*(char *)((int)unaff_A6 + 0x9b) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x9b);
    uVar5 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x9b) = cVar1 + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(char *)((int)unaff_A6 + 0x8b) = *(char *)((int)unaff_A6 + 0x8b) + '\x01';
    *(undefined1 *)((int)unaff_A6 + 0x9b) = 0x1e;
    *unaff_A6 = candidate_d84d4;
    uVar5 = 0;
  }
  candidate_409a();
  if (!(bool)uVar5) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
