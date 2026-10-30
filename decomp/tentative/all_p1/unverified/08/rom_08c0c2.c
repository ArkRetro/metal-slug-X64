/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08c0c2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_8c0c2(void)

{
  undefined4 uVar1;
  short sVar2;
  short sVar3;
  char cVar4;
  char cVar5;

  cVar4 = '\0';
  candidate_288a(0x1c);
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
