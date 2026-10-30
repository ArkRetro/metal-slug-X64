/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08c0cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_8c0cc(void)

{
  undefined4 uVar1;
  short sVar2;
  short sVar3;
  int unaff_A6;
  char cVar4;
  char cVar5;

  if (*(char *)(unaff_A6 + 0x12) != -1) {
    return 0xffffffff;
  }
  if (*(char *)(unaff_A6 + 0x7a) != '\0') {
    return 2;
  }
  cVar4 = '\0';
  candidate_288a();
  cVar5 = '\0';
  uVar1 = candidate_288a();
  sVar3 = CONCAT11(-cVar5,-cVar4);
  if (sVar3 == 0) {
    return 0xffffffff;
  }
  sVar2 = sVar3 + 1;
  if (sVar3 == -1) {
    uVar1 = candidate_29c6();
    sVar2 = (short)uVar1;
  }
  return CONCAT22((short)((uint)uVar1 >> 0x10),(ushort)(byte)-(sVar2 < 0)) & 0xffff0001;
}
