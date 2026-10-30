/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8454.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8454(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  candidate_d9276();
  *(undefined1 *)((int)unaff_A6 + 0x9b) = 2;
  bVar2 = false;
  candidate_427a();
  if ((bVar2) && (*(char *)((int)unaff_A6 + 0x8a) == '\0')) {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
    candidate_d922c();
    *(undefined1 *)(unaff_A6 + 0x21) = 0x83;
    candidate_d9336();
    *unaff_A6 = candidate_d82d6;
  }
  candidate_d92ec();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  uVar3 = false;
  if (*(char *)((int)unaff_A6 + 0x9b) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x9b);
    uVar3 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x9b) = cVar1 + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(char *)((int)unaff_A6 + 0x8b) = *(char *)((int)unaff_A6 + 0x8b) + '\x01';
    *(undefined1 *)((int)unaff_A6 + 0x9b) = 0x1e;
    *unaff_A6 = candidate_d84d4;
    uVar3 = 0;
  }
  candidate_409a();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
