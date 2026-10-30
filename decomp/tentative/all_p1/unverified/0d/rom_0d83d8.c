/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d83d8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d83d8(void)

{
  char cVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  candidate_d9276();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + 1;
  bVar3 = false;
  uVar2 = *(ushort *)((int)unaff_A6 + 0x76) & 1;
  *(ushort *)((int)unaff_A6 + 0x76) = uVar2;
  if (uVar2 == 0) {
    candidate_d9242();
    if ((bVar3) && (*(char *)((int)unaff_A6 + 0x8a) == '\0')) {
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
      candidate_d922c();
    }
  }
  candidate_d92ec();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  uVar4 = false;
  if (*(char *)((int)unaff_A6 + 0x9b) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x9b);
    uVar4 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x9b) = cVar1 + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(char *)((int)unaff_A6 + 0x8b) = *(char *)((int)unaff_A6 + 0x8b) + '\x01';
    *(undefined1 *)((int)unaff_A6 + 0x9b) = 0x1e;
    *unaff_A6 = candidate_d84d4;
    uVar4 = 0;
  }
  candidate_409a();
  if (!(bool)uVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
